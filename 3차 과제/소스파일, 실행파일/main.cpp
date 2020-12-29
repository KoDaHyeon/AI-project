#include "Perceptron.h"
#include <iostream>

using namespace std;

vector<vector<int>> gate_input;
vector<vector<double>> donut_input;

int main()
{
  int l; //layer 수
  int n; //dimension

  cout << "dimension of input? : ";
  cin >> n;

  //gate_input 값 초기화
  //x0=-1, input[i][0]=x1, input[i][1]=x2
  gate_input.assign(pow(2,n), vector<int>(n, 0));
  for(int i=0; i<pow(2,n); i++){
    for(int now=n-1, j=0; now>=0 && j<n; now--, j++){
      if(i & (1 << now)) //i의 now번째 비트가 1이라면
        gate_input[i][j] = 1;
      else gate_input[i][j] = 0;
    }
  }

  //donut_input 값 초기화
  donut_input.assign({
      {0, 0},
      {0, 1},
      {1, 0},
      {1, 1},
      {0.5, 1},
      {1, 0.5},
      {0, 0.5},
      {0.5, 0},
      {0.5, 0.5}
  });

  
  cout << "AND gate learning \n";
  cout << "layer? : ";
  cin >> l;
  Perceptron AND_perceptron(l, n, "AND");
  AND_perceptron.repeat_learning("AND");

  
  cout << "\n OR gate learning \n";
  cout << "layer? : ";
  cin >> l;
  Perceptron OR_perceptron(l, n, "OR");
  OR_perceptron.repeat_learning("OR");


  cout << "\n XOR gate learning \n";
  cout << "layer? : ";
  cin >> l;
  Perceptron XOR_perceptron(l, n, "XOR");
  XOR_perceptron.repeat_learning("XOR");
  

  cout << "\n DONUT data learning \n";
  cout << "layer? : ";
  cin >> l;
  Perceptron DONUT_perceptron(l, n, "DONUT");
  DONUT_perceptron.repeat_learning("DONUT");

  
}