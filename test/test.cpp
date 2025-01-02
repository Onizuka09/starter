/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/31/2024 01:17:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <iostream>
using namespace std;

int *func(int arr[])
{
  arr[0] = 10;
  return arr;
}

int main()
{
  int arr[5] = {1, 2, 4, 5, 4};
  int *tarr = func(arr);
  cout << "%" << 5 << "%";
  printf("%d%",6);
}
