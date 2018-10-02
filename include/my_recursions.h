/*******************************************************************************
 * AUTHOR      : Thuan Tang
 * ID          : 00991588
 * CLASS       : CS008
 * HEADER      : my_recursions
 * DESCRIPTION : This header defines a templated BST (binary search tree) and
 *      its associated functions. The BST can insert and erase a
bst_node::TreeNode at target.
 ******************************************************************************/
#ifndef MY_RECURSIONS_H
#define MY_RECURSIONS_H

#include <iostream>

namespace my_recursions {

// prints out level before/after recursion
void r1_levels(unsigned start_level, unsigned end_level);

// prints out prefix + successive levels of numbers (0 to 9)
void r2_box(std::string prefix, unsigned level);

// prints out permutation of the first word
void r3_permutation(std::string first, std::string second);

// ask user pick prefix within prefix
unsigned r4_i_box(std::string prefix, unsigned level = 0);

// prints ouf the sum of reciprocal of levels
double r5_sumover(unsigned level);

// guess user's number
int r16_guess(unsigned low, unsigned high);

}  // namespace my_recursions

#endif  // MY_RECURSIONS_H
