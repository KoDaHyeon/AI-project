#include <iostream>
#include <vector>
#include <cmath>
#include <random>

using namespace std;
random_device rng;
uniform_real_distribution<double> dist2(-1000.0, 1000.0);  // [-1000.0, 1000.0) 범위 내의 실수

int main()
{
  vector<vector<int>> input;
  vector<double> weight;
  int n; //input은 n차원
  cin >> n;
  input.assign(pow(2,n), vector<int>(n, 0));
  weight.resize(n+1);

  //weight 값 랜덤으로 초기화
  //w0, w1, w2 ->  w0은 고정된 x0(=-1)과 연결, w1은 x1과 연결, w2는 x2와 연결
  cout <<"first weight :" << '\n';
  for(int i=0; i<=n; i++){
    weight[i] = dist2(rng);
    cout << weight[i] << '\n';
  }

  //input 값 초기화
  //input[i][0]=x1, input[i][1]=x2, x0=-1
  for(int i=0; i<pow(2,n); i++){
    for(int j=n-1; j>=0; j++){
      if(i & (1 << j)) //i의 j번째 비트가 1이라면
        input[i][j] = 1;
      else input[i][j] = 0;
    }
  }

  while(true)
  {
    int cnt=0; //cnt : output이 AND게이트의 output과 다른 횟수

    for(int i=0; i<pow(2,n); i++){ //2^n개의 input 세트
      
      //net값 계산 -> output 구함
      double net = -1 * weight[0];
      for(int j=0; j<n; j++){
        net += (weight[j]*input[i][j]);
      }

      int output = (net > 0)? 1 : 0;

      // (output != AND게이트의 output)인 횟수(cnt) 구하기
      int ANDresult = 1; //ANDresult : 원래의 AND게이트 output
      for(int j=0; j<n; j++){
        if(input[i][j] == 0){
          ANDresult = 0; break; //하나라도 0이 나오면 0
        }   
      }
      if(output != ANDresult) cnt++;
    }

    if(cnt == 0){
      //이 경우의 weight, input세트를 이용해 그래프 그림(결과 보고서)
      break;
    }

    cout << "wrong count: " << cnt << '\n'; //output이 틀린 개수 출력
    cout << "please write " << n << " weights." <<'\n'; //새로운 weight값을 입력받음

    //weight값 새로 입력받기
    for(int i=0; i<=n; i++){
      cin >> weight[i];
    }
  }

  //output이 AND게이트의 output과 모두 일치했을 때의 weight값을 출력(그래프를 그리기 위함)
  for(int i=0; i<=n; i++){
    cout << "w" << i << " : " << weight[i] << '\n';
  }
  
  //input세트를 출력(그래프를 그리기 위함)
  cout << "input: \n";
  for(int i=0; i<pow(2,n); i++){
    for(int j=0; j<n; j++){
      cout << input[i][j] << ",";
    }
    cout << '\n';
  }
}
