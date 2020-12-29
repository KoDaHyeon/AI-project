#include "Perceptron.h"
#include <iostream>

using namespace std;

vector<std::vector<int>> input;

int main()
{
  int n; //dimension
  cin >> n;

  //input 값 초기화
  //x0=-1, input[i][0]=x1, input[i][1]=x2
  input.assign(pow(2,n), vector<int>(n, 0));
  for(int i=0; i<pow(2,n); i++){
    for(int now=n-1, j=0; now>=0 && j<n; now--, j++){
      if(i & (1 << now)) //i의 now번째 비트가 1이라면
        input[i][j] = 1;
      else input[i][j] = 0;
    }
  }

  cout << "AND gate learning \n";
  Perceptron AND_perceptron(n, "AND");
  AND_perceptron.repeat_learning();

  cout << "\n OR gate learning \n";
  Perceptron OR_perceptron(n, "OR");
  OR_perceptron.repeat_learning();

  cout << "\n XOR gate learning \n";
  Perceptron XOR_perceptron(n, "XOR");
  XOR_perceptron.repeat_learning();

}