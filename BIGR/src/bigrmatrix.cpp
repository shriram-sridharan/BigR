#include "bigr.h"

extern "C" { // Using this to avoid name mangling.
SEXP bigrmatrix_file(SEXP rows, SEXP cols, SEXP file, SEXP by) {
	SEXP ret;

	PROTECT(ret = R_do_new_object(R_getClassDef("bigrmatrix")));
	R_do_slot_assign(ret, install("filename"), file);
	R_do_slot_assign(ret, install("nrows"), rows);
	R_do_slot_assign(ret, install("ncols"), cols);
	UNPROTECT(1);

	return ret;
}

SEXP show_bigrmatrix(SEXP bigrmatrix) {
	const char * filename =
			CHAR(STRING_ELT(R_do_slot(bigrmatrix, install("filename")), 0));
	double nRows = REAL(R_do_slot(bigrmatrix, install("nrows")))[0];
	double nCols = REAL(R_do_slot(bigrmatrix, install("ncols")))[0];

	Rprintf("FileName : %s nRows : %lf nCols : %lf\n", filename, nRows, nCols);
//	Rprintf("Filename : %s \nData : \n", filename);
//	FILE *fp = fopen(filename, "r");
//	if (fp == NULL) {
//		Rprintf("\nBigRMatrix File : %s not found\n", filename);
//		return R_NilValue;
//	}
//	for (int i = 0; i < nRows; i++) {
//		for (int j = 0; j < nCols; j++) {
//			double f;
//			if (fscanf(fp, "%lf", &f) == EOF) {
//				Rprintf("\nError in File : '%s'. Does not contain %d values\n",
//						filename, nCols * nRows);
//				return R_NilValue;
//			}
//			Rprintf("%lf ", f);
//		}
//		Rprintf("\n");
//	}
//	fclose(fp);

	return R_NilValue;
}
}
