#ifndef _CLIQUERANK_H
#define _CLIQUERANK_H

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <map>
#include <math.h>
#include "../Eigen/Dense"

//using namespace std;
using Eigen::MatrixXd;

class CliqueRankMatrix
{
    public:
        CliqueRankMatrix(double* init_score, int _N, int _Alpha, int _Step, double R)
        {
		
            N = _N;
            Alpha = _Alpha;
            Step = _Step;
		max_R = R;

            edges = new int*[N];
            is_edge = new bool[N*N];
            for(int i = 0; i < N; i++){
                edges[i] = new int[N+1];
                edges[i][N] = 0;
            }

            norm = new double[N];
            p_score = init_score;
	    t_prob = MatrixXd::Zero(N, N);
	    bonus_prob = MatrixXd::Zero(N, N);
	    neighbor = MatrixXd::Zero(N, N);
            p_conf = MatrixXd::Zero(N, N);
	    //for(int i = 0; i < N; i++){
		//for(int j = 0; j < N; j++)
		//p_conf[i] = 0;
	    //}
        }

        void addEdge(int id1, int id2)
        {
            edges[id1][edges[id1][N]] = id2;
            edges[id1][N]++;

            edges[id2][edges[id2][N]] = id1;
            edges[id2][N]++;

            is_edge[id1*N+id2] = true;
            is_edge[id2*N+id1] = true;
        }

        void deleteEdge(int id1, int id2)
        {
            int len = edges[id1][N];
            bool flag = false;
            for(int i = 0; i < len; i++){
                if(flag){
                    edges[id1][i-1] = edges[id1][i];
                }
                if(edges[id1][i] == id2){
                    flag = true;
                }
            }
            edges[id1][N]--;

            is_edge[id1*N+id2] = false;
            is_edge[id2*N+id1] = false;
        }

	void createTransitionMatrix(){
		srand((unsigned)time(NULL));
               	for(int i = 0; i < N; i++){
                        for(int j = 0; j < N; j++){
                                if(norm[i] > 0){
					float r = max_R*(float)rand()/RAND_MAX;
					double old_weight = pow(p_score[i*N+j], Alpha);
					double new_weight = pow((1+r)*p_score[i*N+j], Alpha);

                                        //t_prob(i,j) = (1+r)*pow(p_score[i*N+j], Alpha)/norm[i];
                                        //t_prob(i,j) = old_weight/norm[i];
                                        bonus_prob(i,j) = new_weight/(norm[i]-old_weight+new_weight);
				}
                                else{
                                        t_prob(i,j) = 0;
                                        //bonus_prob(i,j) = 0;
				}
                        }
                }
 
	}


	void init()
	{
		for(int id1 = 0; id1 < N; id1++){
			double nvalue = 0;
			int len = edges[id1][N];
			for(int i = 0; i < len; i++){
				int id2 = edges[id1][i];
				nvalue += pow(p_score[id1*N+id2], Alpha);
				neighbor(id1, id2) = 1;
				neighbor(id2, id1) = 1;
				//nvalue += p_score[id1*N+id2];
			}
			norm[id1] = nvalue;
		}

		// initialize the transition probability matrix
		
		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				if(norm[i] > 0)
					t_prob(i,j) = pow(p_score[i*N+j], Alpha)/norm[i];
				else
					t_prob(i,j) = 0;
			}
		}
		
		createTransitionMatrix();

		// initialize the matrix p_conf
		/*
		for(int id1 = 0; id1 < N; id1++){
			int len = edges[id1][N];
			for(int i = 0; i < len; i++){
				int id2 = edges[id1][i];

				//if(p_score[id1*N+id2] > 0)
				if(norm[id1] > 0){
					p_conf(id1,id2) = pow(p_score[id1*N+id2], Alpha)/norm[id1];
				}
				//if(p_score[id2*N+id1] > 0)
				if(norm[id2] > 0){
					p_conf(id2,id1) = pow(p_score[id2*N+id1], Alpha)/norm[id2];
				}
			}
			//p_conf[id1*N+id2] = p_score[id1*N+id2]/norm[id1];
		}
		*/
	}

	void load(string file)
	{
		ifstream ifile;
		ifile.open(file.c_str());

		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				ifile >> p_conf(i, j);
			}
		}

		ifile.close();
	}

	void iterate()
	{
		init();
		MatrixXd temp_conf = bonus_prob;
		//MatrixXd agg_conf = bonus_prob;
		p_conf = bonus_prob;
		//cout<<"###\t"<<p_conf(877,2069)<<"\t"<<p_conf(2069,877)<<endl;
		ofstream ofile;
	    ofile.open("../../data/test.txt");
	    
            for(int s = 0; s < Step - 1; s++){
	        //cout << "Iteration: " << s << endl;
		//createTransitionMatrix();

	        //for(int m = 0; m < 10; m++){
		//    cout << p_conf(0, m) << " ";
		//}
		//cout << endl;
		temp_conf = t_prob*(temp_conf.cwiseProduct(neighbor));
		p_conf += temp_conf;
            }
	
	    for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
		    if(p_conf(i, j) > 1)
			p_conf(i, j) = 1;
		}
	    }
        }

        int** edges;
        bool* is_edge;

        int N;
        int Alpha;
        int Step;

        double* norm;
        double* p_score;
	double max_R;
        MatrixXd t_prob;
        MatrixXd bonus_prob;
	MatrixXd neighbor;
        MatrixXd p_conf;
};

#endif
