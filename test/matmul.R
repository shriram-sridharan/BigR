library(BIGR)
A <- matrix.bigr(file="test-matrixA", 2,3)
B <- matrix.bigr(file="test-matrixB", 3,2)
C <- A%*%B
