#include "bigr.h"

extern "C" { // Using this to avoid name mangling.

const char * getOutputFileName() {

	stringstream outputfile;
	outputfile << "Output" << time(0);
	return outputfile.str().c_str();
}

SEXP convertStringToSexp(const char * input) {
	SEXP ret;
	PROTECT(ret = allocVector(STRSXP, 1));
	SET_STRING_ELT(ret, 0, mkChar(input));
	UNPROTECT(1);
	return ret;
}

SEXP convertDoubleToSexp(double input) {
	SEXP ret;
	PROTECT(ret = allocVector(REALSXP, 1));
	REAL(ret)[0] = input;
	UNPROTECT(1);
	return ret;
}

SEXP scalarmatmul_bigr(SEXP scalar_sexp, SEXP bigrmatrix) {
	/*
	 * STRSXP is a list of CHARSXPs.
	 * CHARSXP of STRSXP is accessed by using STRING_ELT.
	 * CHAR gets a const char * from character data of CHARSXP.
	 * STRING_ELT(SEXP, position in list)
	 */
	const char * inputfilename = CHAR(
			STRING_ELT(R_do_slot(bigrmatrix, install("filename")), 0));
	double nRows = REAL(R_do_slot(bigrmatrix, install("nrows")))[0];
	double nCols = REAL(R_do_slot(bigrmatrix, install("ncols")))[0];
	double scalar = REAL(scalar_sexp)[0];

	ifstream inputfile(inputfilename, ios::in);
	if (!inputfile.is_open()) {
		Rprintf("\nBigRMatrix File : %s not found\n", inputfilename);
		return R_NilValue;
	}

	const char * outputfilename = getOutputFileName();
	ofstream outputfile(outputfilename, ios::out);
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			if (inputfile.eof()) {
				Rprintf("\nError in File : '%s'. Does not contain %d values\n",
						inputfilename, nCols * nRows);
				outputfile.close();
				remove(outputfilename);

				return R_NilValue;
			}

			double value;
			inputfile >> value;
			outputfile << scalar * value << " ";
		}
		outputfile << "\n";
	}

	outputfile.close();
	inputfile.close();

	SEXP ret;

	PROTECT(ret = R_do_new_object(R_getClassDef("bigrmatrix")));
	R_do_slot_assign(ret, install("filename"),
			convertStringToSexp(outputfilename));
	R_do_slot_assign(ret, install("nrows"), convertDoubleToSexp(nRows));
	R_do_slot_assign(ret, install("ncols"), convertDoubleToSexp(nCols));
	UNPROTECT(1);

	return ret;
}

double seektime = 0.0;

//returns number of bytes seeked. Implementing this using >> is naive and extremely slow.
void seekfile(ifstream &file, double valuestomove) {

	clock_t time = clock();

	char c;

	for (int i = 0; i < valuestomove; i++) {
		// move until character
		do {
			file.get(c);
		} while (c == ' ' || c == '\n');

		// move until space
		do {
			file.get(c);
		} while (c != ' ' && c != '\n');
	}

	seektime = seektime + (double(clock() - time) / CLOCKS_PER_SEC);
}

SEXP matrixmult_bigr(SEXP bigrmatrix1, SEXP bigrmatrix2) {
	const char * mat1_filename = CHAR(
			STRING_ELT(R_do_slot(bigrmatrix1, install("filename")), 0));
	double mat1_nRows = REAL(R_do_slot(bigrmatrix1, install("nrows")))[0];
	double mat1_nCols = REAL(R_do_slot(bigrmatrix1, install("ncols")))[0];

	const char * mat2_filename = CHAR(
			STRING_ELT(R_do_slot(bigrmatrix2, install("filename")), 0));
	double mat2_nRows = REAL(R_do_slot(bigrmatrix2, install("nrows")))[0];
	double mat2_nCols = REAL(R_do_slot(bigrmatrix2, install("ncols")))[0];

	if (mat1_nCols != mat2_nRows) {
		Rprintf("\n Error: Matrix 1 number of cols != Matrix 2 no of Rows\n");
		return R_NilValue;
	}

	ifstream mat1_file(mat1_filename, ios::in);
	if (!mat1_file.is_open()) {
		Rprintf("\nBigRMatrix File : %s not found\n", mat1_filename);
		return R_NilValue;
	}

	ifstream mat2_file(mat2_filename, ios::in);
	if (!mat2_file.is_open()) {
		Rprintf("\nBigRMatrix File : %s not found\n", mat2_filename);
		return R_NilValue;
	}

	const char * outputfilename = getOutputFileName();
	ofstream outputfile(outputfilename, ios::out);

	double curlocinbytes = 0;
	for (int i = 0; i < mat1_nRows; i++) {
		bool newrow = true;
		for (int j = 0; j < mat2_nCols; j++) {
			if (newrow) {
				curlocinbytes = mat1_file.tellg();
				newrow = false;
			} else {
				mat1_file.seekg(curlocinbytes - mat1_file.tellg(),
						ios_base::cur); // seek in reverse direction
			}

			mat2_file.seekg(0, ios_base::beg);
			seekfile(mat2_file, j); // seek to the required column

			double sum = 0;
			for (int k = 0; k < mat1_nCols; k++) {
				double mat1value, mat2value;
				mat1_file >> mat1value;
				mat2_file >> mat2value;

				sum = sum + mat1value * mat2value;

//				Rprintf("1 file i=%d, k =%d val : %lf\n", i, k, mat1value);
//				Rprintf("2 file k=%d, j =%d val : %lf\n", k, j, mat2value);

				if (k != mat1_nCols - 1) // should not seek beyond the last character. [mat1_nCols = mat2_nRows]
					seekfile(mat2_file, mat2_nCols - 1); // seek by number of columns in the second matrix.
			}
			outputfile << sum << " ";
		}
		outputfile << "\n";
	}

	outputfile.close();
	mat1_file.close();
	mat2_file.close();

	SEXP ret;

	PROTECT(ret = R_do_new_object(R_getClassDef("bigrmatrix")));
	R_do_slot_assign(ret, install("filename"),
			convertStringToSexp(outputfilename));
	R_do_slot_assign(ret, install("nrows"), convertDoubleToSexp(mat1_nRows));
	R_do_slot_assign(ret, install("ncols"), convertDoubleToSexp(mat2_nCols));
	UNPROTECT(1);

	Rprintf("Seek time %lf", seektime);
	return ret;
}

}
