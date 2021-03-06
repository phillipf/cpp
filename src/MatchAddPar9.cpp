#include <Rcpp.h>
#include <boost/bind.hpp>
#include "edlib.h"

// [[Rcpp::depends(RcppParallel)]]
// [[Rcpp::depends(BH)]]
// [[Rcpp::plugins("cpp11")]]
#include <RcppParallel.h>
using namespace RcppParallel;

inline double match(char* x, char* y) {

  int lenx = std::string(x).length();
  int leny = std::string(y).length();

  EdlibAlignResult result = edlibAlign(x, lenx, y, leny, edlibDefaultAlignConfig());

  return double(result.editDistance);

};

struct MatchAddress : public Worker
{
  // source matrix
  Rcpp::CharacterVector input;
  Rcpp::CharacterVector target;

  // destination matrix
  RMatrix<double> output;

  // initialize with source and destination
  MatchAddress(Rcpp::CharacterVector input, Rcpp::CharacterVector target, Rcpp::NumericMatrix output)
    : input(input), target(target), output(output) {}

  // take the square root of the range of elements requested
  void operator()(std::size_t begin, std::size_t end) {

    for (std::size_t i = begin; i < end; i++) {

         Rcpp::CharacterVector::iterator tar = target.begin();
         Rcpp::CharacterVector::iterator tarend = target.end();

          // char* t = *tar;

          while(tar != tarend) {

            char* t = *tar++;

            std::transform(target.begin(),
                           target.end(),
                           output.row(i).begin(),
                           boost::bind(&match, _1, t));
    }

  }
}
};


// [[Rcpp::export]]
Rcpp::NumericMatrix parallelMatrixSqrt(Rcpp::CharacterVector input, Rcpp::CharacterVector target) {

  // allocate the output matrix
  Rcpp::NumericMatrix output(input.size(), target.size());

  // SquareRoot functor (pass input and output matrixes)
  MatchAddress Matchaddress(input, target, output);

  // call parallelFor to do the work
  parallelFor(0, input.size(), Matchaddress);

  // return the output matrix
  return output;
}


/*** R
library(stringi)
set.seed(2)
input <- stringi::stri_rand_strings(10, 5)
target <- stringi::stri_rand_strings(5500, 5)
target[11] <- input[1]

result <- parallelMatrixSqrt(target, target)
#result <- parallelMatrixSqrt(c("A", "B", "ABCCE"), c("AB", "B", "ABCCD"))

*/
