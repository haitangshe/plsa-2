/*
 * Name     : plsa.cpp
 * Desc     : plsa program main
 * Time     : 2014-08-14
 * Author   : wangxiaofeng
 */

#include "plsa.h"

PLSA::~PLSA() {

}

void PLSA::generate_cooc(const char*file_name) {
    ifstream is(file_name);
    string line;
    int d_idx = 0;
    int w_idx = 0;
    while (getline(is ,line)) {
        vector<string> seged_words = seg_words(line.c_str());
        for (int i = 0; i < seged_words.size(); i ++) {
            if (word_to_index.find(seged_words[i]) == word_to_index.end()) {
                index_to_word.push_back(seged_words[i]);
                word_to_index.insert(pair<string, int>(seged_words[i], w_idx));
                cooc[d_idx][w_idx] += 1;
                w_idx += 1;
            }
            else {
                cooc[d_idx][word_to_index[seged_words[i]]] += 1;
            }
        }
        doc_word[d_idx] = seged_words.size();
        index_to_doc.push_back(line);
        d_idx += 1;
    }
    d_n = d_idx;
    w_n = w_idx;        
    z_n = 10;
}

void PLSA::e_step() {
    for (int i = 0; i < d_n; i ++) {
        for (int k = 0 ; k < z_n; k ++) {
            last_p_z_d[k][i] = p_z_d[k][i];
        }
    }
    for (int j = 0; j < w_n; j ++) {
        for (int k = 0; k < z_n; k ++) {
            last_p_w_z[j][k] = p_w_z[j][k];
        }
    }
}

void PLSA::m_step() {
    float tt = 0.0;
    for (int j = 0; j < w_n; j ++) {
        for (int k = 0; k < z_n; k ++) {
            float t_i = 0.0, t_j = 0.0;
            for (int i = 0; i < d_n; i ++) {
                float top = cooc[i][j] * last_p_w_z[j][k] * last_p_z_d[k][i];
                float down = 0.0;
                for (int k1 = 0; k1 < z_n; k1 ++) {
                    down += last_p_w_z[j][k1] * last_p_z_d[k1][i];
                }
                if (top != 0 && down != 0)
                    t_i += top / down;
            }
            
            for (int j1 = 0; j1 < w_n; j1 ++) {
                for (int i1 = 0; i1 < d_n; i1 ++) {
                    float top = cooc[i1][j1] * last_p_w_z[j1][k] * last_p_z_d[k][i1];
                    float down = 0.0;
                    for (int k1 = 0; k1 < z_n; k1 ++) {
                        down += last_p_w_z[j1][k1] * last_p_z_d[k1][i1];
                    }
                    if (top == 0 || down == 0)
                        t_j += 0;
                    else
                        t_j += top / down;
                }
            }
            if (t_i == 0 || t_j == 0)
                p_w_z[j][k] = 0.0;
            else
                p_w_z[j][k] = t_i / t_j;
        }
    }
    
    for (int i = 0; i < d_n; i ++) {
        for (int k = 0; k < z_n; k ++) {
            float t = 0.0;
            for (int j = 0; j < w_n; j ++) {
                float top = cooc[i][j] * last_p_w_z[j][k] * last_p_z_d[k][i];
                float down = 0.0;
                for (int k1 = 0; k1 < z_n; k1 ++) {
                    down += last_p_w_z[j][k1] * last_p_z_d[k1][i];
                }
                if (top != 0 && down != 0)
                    t += top / down;
            }
            p_z_d[k][i] = t / doc_word[i];
        }
    }
    
}

float PLSA::likelyhood() {
    float val = 0.0;
    for (int i = 0; i < d_n; i ++) {
        for (int j = 0; j < w_n; j ++) {
            float t = 0.0;
            for (int k = 0; k < z_n; k ++) {
                t += p_z_d[k][i] * p_w_z[j][k];
            }
            if (t != 0)
            val += cooc[i][j] * log(t);
        }
    }
    return val;
}

void PLSA::init_params() {
    for (int i = 0; i < d_n; i ++) {
        srand(time(NULL));
        for (int k = 0; k < z_n; k ++) {
            p_z_d[k][i] = rand() % 100 / 100.0;
        }
    }
    for (int j = 0; j < w_n; j ++) {
        srand(time(NULL));
        for (int k = 0; k < z_n; k ++) {
            p_w_z[j][k] = rand() % 100 / 100.0;
        }
    }
}

void PLSA::plsa_training() {
    int cur_iter = 0;
    float last_likelyhood = 10000000;
    init_params();
    while (cur_iter ++ < MAX_ITER) {
        e_step();
        m_step();
        float hood_val = likelyhood();
        if (abs(last_likelyhood - hood_val) < eps) {
            break;
        }
        last_likelyhood = hood_val;
        cout << last_likelyhood << endl;
    }
    save_model();
}


void PLSA::init_env(const char*path) {
    
}

vector<string> PLSA::seg_words(const char*words) {
}

void PLSA::save_model() {

    for (int j = 0; j < d_n; j ++) {
        cout << index_to_doc[j] << endl;
        for (int k = 0; k < z_n; k ++) {
            cout << k << ":" << p_z_d[k][j] << endl;
        }
    }

    for (int k = 0; k < z_n; k ++) {
        cout << k << ":" << endl;
        float t[10001];
        sort(t, t + w_n);
        for (int j = 0; j < w_n; j ++) {
            cout << index_to_word[j] << ":";
            cout << p_w_z[j][k] << endl;
        }
    }
}

int main(int argc, char *argv[]) {
    PLSA *plsa = new PLSA();
    plsa->init_env("chinese_gbk");
    plsa->generate_cooc(argv[1]);
    plsa->plsa_training();
    return 0;
}
