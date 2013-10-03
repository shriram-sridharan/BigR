.onAttach <- function(libname, pkgname) {
  cat("Using BigR\n")
}

# S4 class and method definitions 
.onLoad = function(libname, pkgname) { 
  setClass("bigrmatrix", representation(nrows="numeric",ncols="numeric",filename="character"))
  setMethod("show", signature("bigrmatrix"), show_bigrmatrix)
  setMethod("*", signature("numeric","bigrmatrix"), scalarmatmul_bigr) #scalar matrix multiplication
  setMethod("%*%", signature(x="bigrmatrix", y="bigrmatrix"), matrixmult_bigr) #matrix multiplication
}

matrix.bigr <- function(nrows, ncols, file, byRow=FALSE) {
  .Call("bigrmatrix_file", nrows, ncols, file, byRow)
}

show_bigrmatrix <- function(object) {
  .Call("show_bigrmatrix", object)
}

scalarmatmul_bigr <- function(e1, e2) {
  .Call("scalarmatmul_bigr", e1, e2)
}

matrixmult_bigr <- function(x, y) {
  .Call("matrixmult_bigr", x, y)
}


