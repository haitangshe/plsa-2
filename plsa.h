/*
 * Name     : plsa.cpp
 * Desc     : plsa trainning and loading para
 * Time     : 2014-08-20
 * Author   : wangxiaofeng
 */

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <time.h>
#include <map>
#include <vector>
#define Z_MAX 1000
#define D_MAX 1000
#define W_MAX 1000
#define MAX_ITER 100
#define MAX_TOKEN_SIZE 1024
#define eps 1.0e-5
using namespace std;


class PLSA {
    private:
        float p_z_d[Z_MAX][D_MAX] ; 
        float p_w_z[W_MAX][Z_MAX] ;
        float p_w[W_MAX] ;
        float p_d[W_MAX] ;
        
        float last_p_d[D_MAX];
        float last_p_z_d[Z_MAX][D_MAX];
        float last_p_w_z[W_MAX][Z_MAX];
        
        float t[D_MAX][W_MAX];
        //float p_w_d[W_MAX][D_MAX] ;
        map<string, int> word_to_index ;
        vector<string> index_to_word ;
        int d_n, w_n, z_n;
        int doc_word[D_MAX];
        vector<string> index_to_doc;
        int cooc[D_MAX][W_MAX];
        scw_worddict_t *pwdict;
        scw_out_t *pout;
        string dict_path;
    public:
        PLSA(){}
        ~PLSA();
        void init_env(const char*path);
        vector <string> seg_words(const char*words);
        float likelyhood();
        void generate_cooc(const char *file_name) ;
        void e_step() ;
        void save_model();
        void m_step() ;
        void plsa_training() ;
        void init_params();
        void sim(const char*query1, const char*query2) ;
};
