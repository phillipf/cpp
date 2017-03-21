#include <Rcpp.h>
#include <boost/bind.hpp>
#include "edlib.h"

// [[Rcpp::depends(RcppParallel)]]
// [[Rcpp::depends(BH)]]
// [[Rcpp::plugins("cpp11")]]
#include <RcppParallel.h>
using namespace RcppParallel;

inline double match(char* x, char* y) {

  int lenx = str::to_string(x).length();
  int leny = str::to_string(y).length();

  EdlibAlignResult result = edlibAlign(x, lenx, y, leny, edlibDefaultAlignConfig());

  return double(result.editDistance)

}

struct MatchAddress : public Worker
{
  // source matrix
  const CharacterVector input;
  const CharacterVector target;

  // destination matrix
  RMatrix<double> output;

  // initialize with source and destination
  SquareRoot(const CharacterVector input, const CharacterVector target)
    : input(input), target(target), output(output) {}

  // take the square root of the range of elements requested
  void operator()(std::size_t begin, std::size_t end) {
    std::transform(input.begin() + begin,
                   input.begin() + end,
                   output.begin() + begin,
                   ::sqrt);
  }
};


// [[Rcpp::export]]
NumericMatrix parallelMatrixSqrt(NumericMatrix x) {

  // allocate the output matrix
  NumericMatrix output(x.nrow(), x.ncol());

  // SquareRoot functor (pass input and output matrixes)
  SquareRoot squareRoot(x, output);

  // call parallelFor to do the work
  parallelFor(0, x.length(), squareRoot);

  // return the output matrix
  return output;
}


// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically
// run after the compilation.
//

/*** R
parallelMatrixSqrt(42)
*/
