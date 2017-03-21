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


// [[Rcpp::export]]
Rcpp::NumericMatrix MatchAddress(Rcpp::CharacterVector input, Rcpp::CharacterVector target) {
  // std::transform([...], boost::bind(&f, _1, param1, param2, param3));

  // RMatrix<double> output(input.size(), target.size());

  Rcpp::NumericMatrix output(input.size(), target.size());

  for(int i = 0; i < input.size(); ++i) {

    Rcpp::CharacterVector::iterator tar = target.begin();
    // Rcpp::CharacterVector::iterator tarend = target.end();

    // char* t = *tar

    while(tar != target.end()) {

      char* t = *tar++;
      // char* inp = input[i];
      // double out = *output++

      // output.row(i) = match();

      std::transform(target.begin(),
                     target.end(),
                     output.row(i).begin(),
                     boost::bind(&match, _1, t));
    }

    return output;
    }
};

// struct MatchAddress : public Worker
// {
//   // source matrix
//   Rcpp::CharacterVector input;
//   Rcpp::CharacterVector target;
//
//   // destination matrix
//   RMatrix<double> output;
//
//   // initialize with source and destination
//   MatchAddress(Rcpp::CharacterVector input, Rcpp::CharacterVector target)
//     : input(input), target(target), output(output) {}
//
//   // take the square root of the range of elements requested
//   void operator()(std::size_t begin, std::size_t end) {
//     // std::transform([...], boost::bind(&f, _1, param1, param2, param3));
//
//     // RMatrix<double> output(input.size(), target.size());
//
//     Rcpp::CharacterVector::iterator tar = target.begin();
//     Rcpp::CharacterVector::iterator tarend = target.end();
//
//     // char* t = *tar
//
//     while(tar != tarend) {
//
//       char* t = *tar++;
//
//       std::transform(input.begin() + begin,
//                      input.begin() + end,
//                      output.begin() + begin,
//                      boost::bind(&match, _1, t));
//     }
//   }
// };



// Rcpp::NumericMatrix parallelMatrixSqrt(Rcpp::CharacterVector input, Rcpp::CharacterVector target) {
//
//   // allocate the output matrix
//   Rcpp::NumericMatrix output(input.size(), target.size());
//
//   // SquareRoot functor (pass input and output matrixes)
//   MatchAddress Matchaddress(input, target);
//
//   // call parallelFor to do the work
//   parallelFor(0, input.size(), Matchaddress);
//
//   // return the output matrix
//   return output;
// }

// Rcpp::NumericMatrix parallelMatrixSqrt(Rcpp::CharacterVector input, Rcpp::CharacterVector target) {
//
//   // allocate the output matrix
//   Rcpp::NumericMatrix output(input.size(), target.size());
//
//   // SquareRoot functor (pass input and output matrixes)
//   MatchAddress Matchaddress(input, target);
//
//   // return the output matrix
//   return output;
// }



// You can include R code blocks in C++ files processed with sourceCpp
// (useful for testing and development). The R code will be automatically
// run after the compilation.
//

/*** R
library(stringi)
set.seed(2)
input <- stringi::stri_rand_strings(10, 5)
target <- stringi::stri_rand_strings(10, 5)
result <- MatchAddress(input, target)
*/
