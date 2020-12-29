#ifndef __PERCEPTRON_H__
#define __PERCEPTRON_H__

#include <vector>
#include <cmath>
#include <random>
#include <iostream>


const double C = 0.1; //learning rate
extern std::vector<std::vector<int>> gate_input;
extern std::vector<std::vector<double>> donut_input;
std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_real_distribution<double> dis(-1.0, 1.0);  // [-1.0, 1.0) 범위 내의 실수

class Perceptron
{
  private:
    int layer; //layer 수
    int layer_of_node; //layer 당 node 수
    int dimension; //input의 차원 수
    std::vector<std::vector<std::vector<double>>> weight;
    std::vector<int> target;
    std::vector<double> error;
    std::vector<double> ex_error;
    std::vector<double> error_diff;

  public:
    Perceptron(int l, int n, std::string TYPE);
    double activation(double net);
    double calculate(int test_case, std::string TYPE, std::vector<std::vector<double>>&net, std::vector<std::vector<double>>&tmp_input);
    void learning(int test_case, std::string TYPE, std::vector<std::vector<double>>&net, std::vector<std::vector<double>>&tmp_input);
    bool isLearned();
    void repeat_learning(std::string TYPE);
    double D_activation(double y);

};


Perceptron::Perceptron(int l, int n, std::string TYPE)
{
  layer = l;
  dimension = n;
  layer_of_node = dimension;
  //layer_of_node = dimension일때만 작동하도록 구현함.

  //weight를 랜덤으로 초기화
  weight.assign(layer, std::vector<std::vector<double>>(dimension+1, std::vector<double>(layer_of_node, 0)));             

  for(int k=0; k<layer; k++){
    for(int i=0; i<=dimension; i++){
      for(int j=0; j<layer_of_node; j++){
        weight[k][i][j] = double(dis(gen));
      }
    }
  }
 

  //AND gate의 target 계산
  if (TYPE == "AND"){
    target.resize(pow(2, n), 1);
    for (int i = 0; i < pow(2, n); i++){
      for (int j = 0; j < n; j++){
        if (gate_input[i][j] == 0){ //하나라도 0이면 target은 0
          target[i] = 0;
          break;
        }
      }
    }
  }

  //OR gate의 target 계산
  else if (TYPE == "OR"){
    target.resize(pow(2, n), 0);
    for (int i = 0; i < pow(2, n); i++){
      for (int j = 0; j < n; j++){
        if (gate_input[i][j] == 1){ //하나라도 1이면 target은 1
          target[i] = 1;
          break;
        }
      }
    }
  }

  //XOR gate의 target 계산
  else if (TYPE == "XOR"){
    target.resize(pow(2, n), 0);
    for (int i = 0; i < pow(2, n); i++){
      int cntOf1 = 0; //1의 개수
      for (int j = 0; j < n; j++){
        if (gate_input[i][j] == 1) cntOf1++;
      }
      if (cntOf1 % 2 != 0) target[i] = 1; //1의 개수가 홀수개면 target은 1
    }
  }

  //DONUT 데이터의 target 계산
  else if (TYPE == "DONUT"){
    target.assign({0,0,0,0,0,0,0,0,1});
  }
}

double Perceptron::activation(double net)
{
  //return 1 / (1+(double)exp(-net)); //sigmoid
  return(net > 0)? net : (double)exp(net)-1; //ELU
}



/* test_case번째 test data의 output을 계산 */
double Perceptron::calculate(int test_case, std::string TYPE, std::vector<std::vector<double>>&net, std::vector<std::vector<double>>&tmp_input)
{

  net.assign(layer, std::vector<double>(layer_of_node, 0));
  tmp_input.assign(layer, std::vector<double>(layer_of_node, 0));

 
  if(TYPE == "DONUT"){
    for(int i=0; i<dimension; i++){
      tmp_input[0][i] = donut_input[test_case][i];
    }
  }
  else{ //TYPE == "AND", "OR", "XOR"
    for(int i=0; i<dimension; i++){
      tmp_input[0][i] = gate_input[test_case][i];
    }
  }

  if (layer > 1)
  {
    for (int k = 0; k < layer - 1; k++){
      for (int j = 0; j < layer_of_node; j++){
        for (int i = 0; i <= dimension; i++){
          if (i == 0){
            net[k][j] += weight[k][i][j] * (-1);
          }
          else{
            net[k][j] += weight[k][i][j] * tmp_input[k][i - 1];
          }
        }
      }
      for (int j = 0; j < layer_of_node; j++){
        tmp_input[k][j] = activation(net[k][j]);
      }
    }
  }

    //마지막 layer일때(k=layer-1)
    for(int i=0; i<=dimension; i++){
      if(i==0){
        net[layer-1][0] += weight[layer-1][i][0] * (-1);
      }
      else{
        net[layer-1][0] += weight[layer-1][i][0] * tmp_input[layer-1][i-1];
      }
    }

    return activation(net[layer-1][0]);
}

double Perceptron::D_activation(double y)
{
  return(y>0)? 1 : y+1; //ELU
}


/* test_case번째 test data를 가지고 weight값을 training */
void Perceptron::learning(int test_case, std::string TYPE, std::vector<std::vector<double>> &net, std::vector<std::vector<double>> &tmp_input)
{

  std::vector<std::vector<double>> delta(layer, std::vector<double>(layer_of_node, 0));

  for (int k = layer - 1; k >= 0; k--){


    if (k == layer - 1){
      delta[k][0] = D_activation(activation(net[k][0])) * (-(target[test_case] - activation(net[k][0])));
      for (int i = 0, j = -1; i <= dimension; i++, j++){
        if (i == 0){
          weight[k][i][0] += C * delta[k][0] * (-1);
          
        }
        else{
          if(layer > 1){
            weight[k][i][j] += C * delta[k][0] * tmp_input[k - 1][j];
            delta[k - 1][j] = D_activation(activation(net[k][0])) * weight[k][i][j] * delta[k][0];
          }
          else{ //layer==1(k=0)
            if(TYPE == "DONUT"){
              weight[k][i][j] += C * delta[k][0] * donut_input[test_case][j];
            }
            else{
              weight[k][i][j] += C * delta[k][0] * gate_input[test_case][j];
            }
          }
        }
      }
    }


    else if (k > 0){
      for (int i = 0; i <= dimension; i++){
        for (int j = 0; j < layer_of_node; j++){
          if (i == 0){
            weight[k][i][j] += C * delta[k][j] * (-1);
          }
          else{
            weight[k][i][j] += C * delta[k][j] * tmp_input[k - 1][i];
          }
        }
        if (i > 0){
          delta[k - 1][i - 1] = D_activation(activation(net[k - 1][i - 1]));
          double tmp = 0;
          for (int j = 0; j < layer_of_node; j++){
            tmp += weight[k][i][j] * delta[k][j];
          }
          delta[k - 1][i - 1] *= tmp;
        }
      }
    }


    else{ //k==0
      for (int i = 0; i <= dimension; i++){
        for (int j = 0; j < layer_of_node; j++){
          if (i == 0){
            weight[0][i][j] += C * delta[0][j] * (-1);
          }
          else{ //i>0
            weight[0][i][j] += C * delta[0][j] * gate_input[test_case][i - 1];
          }
        }
      }
    }


  }
}

/* while문을 한 번 돌았을 때 학습에 성공(각 test case의 error값이 0.01 미만)했는지 알려주는 함수*/
bool Perceptron::isLearned(void)
{
  bool islearned = true;
  for (int i = 0; i < target.size(); i++)
  {
    if (error[i] < 0.01)
      continue;
    else
    {
      islearned = false;
      break;
    }
  }
  return islearned;
}

/*while문을 반복*/
void Perceptron::repeat_learning(std::string TYPE)
{

  bool learningable = true;
  int round = 1;
  error.resize(target.size()); //error는 output개수(=target 개수)만큼 존재
  ex_error.resize(target.size());
  error_diff.resize(target.size());

  while(learningable)
  {
    if(TYPE == "DONUT"){
      for (int i = 0; i < 9; i++){ //for each training data(i번째 test case)
        std::vector<std::vector<double>> net; //k번째 layer의 layer_of_node번째 노드의 net값
        std::vector<std::vector<double>> tmp_input; //k번째 layer의 layer_of_node번째 노드로 들어오는 input값
        double output = calculate(i, TYPE, net, tmp_input);
        std::cout << output << '\n';
        ex_error[i] = 1 / 2 * pow(target[i] - output, 2); //i번째 test case의 업뎃 전 error값

        //weight 업데이트
        learning(i, TYPE, net, tmp_input);

        error[i] = pow(target[i] - output, 2); //i번째 test case의 업뎃 후 error값
        error_diff[i] = error[i] - ex_error[i]; //업뎃 전, 후 error값 차이
      }

      //feedback
      if (isLearned() == true){ //error가 일정 값 이하일 때
        std::cout << "Learned successfully at " << round << "epoch.\n";
        return;
      }
      else{
        //끝낼지 말지 결정
        double error_diff_average = 0;
        for (int i = 0; i < 9; i++){
          error_diff_average += error_diff[i];
        }
        error_diff_average /= 9;
        if (error_diff_average < 0.01 || round > 3000){ //학습 불가 판단(학습 전후 error값 차이<0.01 || epoch>3000)
          std::cout << "Learning failed at " << round << "ecpoch.\n";
          learningable = false;
        }
      }
      round++;
    }

    else{ //TYPE == "AND", "OR", "XOR"
      for (int i = 0; i < pow(2, dimension); i++){ //for each training data(i번째 test case)
        std::vector<std::vector<double>> net; //k번째 layer의 layer_of_node번째 노드의 net값
        std::vector<std::vector<double>> tmp_input; //k번째 layer의 layer_of_node번째 노드로 들어오는 input값
        double output = double(calculate(i, TYPE, net, tmp_input));
        
        ex_error[i] = double(pow(target[i] - output, 2)); //i번째 test case의 업뎃 전 error값
    

        //weight 업데이트
        learning(i, TYPE, net, tmp_input);

        output = calculate(i, TYPE, net, tmp_input);

        error[i] = double(pow(target[i] - output, 2)); //i번째 test case의 업뎃 후 error값
        

        error_diff[i] = error[i] - ex_error[i];
        error_diff[i] = (error_diff[i] > 0)? error_diff[i] : -error_diff[i];//업뎃 전, 후 error값 차이

    
      }


      //feedback
      if(isLearned() == true){ //error가 일정 값 이하일 때
        std::cout << "Learned successfully at " << round << "epoch.\n";
        return;
      }
      else{
        //끝낼지 말지 결정
        double error_diff_average = 0;
        for(int i=0; i<pow(2, dimension); i++){
          error_diff_average += error_diff[i];
        }
        error_diff_average /= pow(2, dimension);
      
        if(error_diff_average < 0.00001 || round > 3000){ //학습 불가 판단
          std::cout << "Learning failed at " << round << "ecpoch.\n" ;
          learningable = false;
        }
      }

    round++;
    }
  }

}

#endif