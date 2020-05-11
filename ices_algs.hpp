///////////////////////////////////////////////////////////////////////////////
// crossing_algs.hpp
//
// Algorithms that solve the iceberg avoiding problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on ices_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <iostream>

#include <math.h>

#include "ices_types.hpp"

namespace ices {
// Solve the iceberg avoiding problem for the given grid, using an exhaustive
// optimization algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
  unsigned int iceberg_avoiding_exhaustive(const grid& setting) {
      
    // grid must be non-empty.
    assert(setting.rows() > 0);
    assert(setting.columns() > 0);

    // Compute the path length, and check that it is legal.
    const size_t steps = setting.rows() + setting.columns() - 2;
    assert(steps < 64);

    unsigned int count_paths = 0;
   
    bool isValid;
    path candidate(setting); // initialize candidate with setting
    for(int bits = 0; bits <= (pow(2, steps))-1; bits++) {
      candidate = setting; // row = column = 0 which is the equivalent of the start, [0][0]
      for(int k = 0; k <= (steps-1); k++) {
        isValid = true;
        int bit = (bits >> k) & 1;
        if(bit == 1) {
          // step must be validated before being added to the path
          if(!(candidate.is_step_valid(STEP_DIRECTION_RIGHT))) {
            isValid = false; 
            break; 
          }
          candidate.add_step(STEP_DIRECTION_RIGHT);
        }
        else {
          // step must be validated before being added to the path
          if(!(candidate.is_step_valid(STEP_DIRECTION_DOWN))) {
            isValid = false; 
            break; 
          }
          candidate.add_step(STEP_DIRECTION_DOWN);
        }
      }

      // if all the steps were valid, the path is valid, so we can increment
      if(isValid == true) {
        count_paths++;
      }

    }
    
    return count_paths;
  }

// Solve the iceberg avoiding problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
  unsigned int iceberg_avoiding_dyn_prog(const grid& setting) {

    // grid must be non-empty.
    assert(setting.rows() > 0);
    assert(setting.columns() > 0);


    const int DIM=100;
    std::vector<std::vector<unsigned>> A(DIM, std::vector<unsigned>(DIM));
    // Base case
    A[0][0] = 1;

    // General case
    int from_above, from_left;
    for(int i = 0; i <= setting.rows()-1; i++) {
      for(int j = 0; j <= setting.columns()-1; j++) {
        // Base case already addresses A[0][0] in order to ensure that it is not an iceberg, so we can skip it
        if(i == 0 && j == 0) {
          continue;
        }
        // this checks to see if the cell is at [i][j] is an iceberg or not
        if(setting.may_step(i, j) == false) { 
          A[i][j] = 0;
          continue; 
        }
        from_above = from_left = 0;

        // Checking above the current cell
        if(i > 0 && (A[i-1][j] != 0)) {
          from_above = A[i-1][j];
        }

        // Checking to the left of the current cell 
        if(j > 0 && (A[i][j-1] != 0)) {
          from_left = A[i][j-1];
        }

        A[i][j] = from_above + from_left;

        if(from_above == 0 && from_left == 0) {
          A[i][j] = 0;
        }

      }
    }
    
    return A[setting.rows()-1][setting.columns()-1];
  }

}
