//
// Created by MI on 2024/4/2.
//
//
// Created by MI on 2024/4/2.
//
//
// Created by root on 3/29/24.
//
#include <iostream>
#include <cstdlib>
int players[2][7] = {{4,4,4,4,4,4,0},{4,4,4,4,4,4, 0}};
int now = 0;
int last = 0;
extern "C" {
int mancala_result_ptr(int *flag_ptr, int *seq, int *size_ptr) {
    for(int i = 0; i < 2; i++) {
        for(int j = 0; j < 7; j++) {
            players[i][j] = (j == 6) ? 0 : 4;
            now = 0;
            last = 0;
        }
    }
    int flag = *flag_ptr;
    int size = *size_ptr;
    last = 3 - flag;
    now = flag;
    for (int i = 0; i < size; i++) {
        // 检查顺序
        //printf("seq[%d] = %d\n", i , seq[i]);
        int real = 11 <= seq[i] && seq[i] <= 16 ? 1 : 2;
        if (real != now || last == now) {
            return 30000 + i;
        }
        bool ccheck_flag = true;
        for (int j = 0; j < 6; j++) {
            ccheck_flag = players[(3 - now) - 1][j] == 0 && ccheck_flag;
        }
        if (ccheck_flag) {
            return 30000 + i;
        }
        // 计算分数
        int pos_2 = seq[i] % 10 - 1;
        int pos_1 = real;
        int number = players[real - 1][pos_2];
        players[real - 1][pos_2] = 0;
        // 检查是否对无棋子区域操作
        if (number == 0) {
            return 30000 + i;
        }
        while (number > 0) {
            pos_2 = pos_2 + 1;
            if (pos_2 > 6) {
                pos_1 = 3 - pos_1;
                pos_2 = 0;
            }
            if (pos_1 != real && pos_2 == 6) continue;
            players[pos_1 - 1][pos_2] += 1;
            number--;
            if (number == 0) {
                if (pos_2 != 6 || pos_1 != real) {
                    //交换顺序
                    last = now;
                    now = 3 - now;
                }
                if (0 <= pos_2 && pos_2 <= 5 && pos_1 == real) {
                    //计算是否可以吞并
                    int target_pos = 5 - pos_2;
                    if (players[pos_1 - 1][pos_2] == 1 && players[(3 - pos_1) - 1][target_pos] != 0) {
                        players[pos_1 - 1][6] += players[(3 - pos_1) - 1][target_pos];
                        players[pos_1 - 1][6] += players[pos_1 - 1][pos_2];
                        players[pos_1 - 1][pos_2] = 0;
                        players[(3 - pos_1) - 1][target_pos] = 0;
                    }
                }
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        int check_player = 2 - i;
        bool check_flag = true;
        for (int j = 0; j < 6; j++) {
            check_flag = players[check_player - 1][j] == 0 && check_flag;
        }
        if (check_flag) {
            int target_player = 3 - check_player;
            for (int j = 0; j < 6; j++) {
                players[target_player - 1][6] += players[target_player - 1][j];
                players[target_player - 1][j] = 0;
            }
            return 15000 + players[flag - 1][6] - players[3 - flag - 1][6];
        }
    }
    return 20000 + players[flag - 1][6];
}
}
