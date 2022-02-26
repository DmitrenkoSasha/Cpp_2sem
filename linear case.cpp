//
// Created by пк on 21.02.2022.
//

#include <iostream>
#include <fstream>
#include <random> //random engine, random distribution
#include <chrono>

bool maybe(double prob,
           std::default_random_engine &rnd_eng,
           std::uniform_real_distribution<double> &dstr);

void swaparray(int m[], int size,
               std::default_random_engine &rnd_eng,
               std::uniform_int_distribution<size_t> &UID);

using namespace std;

void linear(int w, int arr_av[]){
    auto current_time =
            chrono::system_clock::now().time_since_epoch();
    auto seed =
            std::chrono::duration_cast<std::chrono::seconds>(current_time).count();

    std::default_random_engine rnd_eng(seed);
    std::uniform_real_distribution<double> dstr(0.0,1.0);
    std::uniform_int_distribution<size_t> forSwap(0,w);

    int h=1;
    int arr[h][w]; // массив, представляющий кристалл
    int i, j;
    int amount_dis = 0;


    for (int p=1; p<w; p++) {
        std::ofstream out;          // поток для записи
        int sum = 0;
        for (int z = 0; z < 40; z++) {

            // заполняю клеточку кристалла атомом(0) или пустотой(1)
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {
                    arr[i][j] = maybe(p * 0.01, rnd_eng, dstr);
                    amount_dis += arr[i][j];
                    //cout << arr[i][j] << ' ';
                }
                //cout << '\n';
            }
            //cout << '\n';

            int coord_hor[amount_dis]; // массив с гориз коорд единиц
            int coord_ver[amount_dis]; // с вертикальными коорд.

            // заполняю массивы координат единиц
            int k = 0; // индекс эл-ов массивов координат
            for (j = 0; j <= w; j++) {
                for (i = 0; i <= h; i++) {
                    if (arr[i][j] == 1) {
                        coord_hor[k] = i;
                        coord_ver[k] = j;
                        k += 1;
                    }

                }
            }
            int counter = 0;

            while (amount_dis != 0) {
                counter++;
                // ПРОВЕРЯЮ НА СОСЕДСТВА ПУСТОТ, СОСЕДСТВА С ГРАНИЦЕЙ
                for (i = 0; i < amount_dis; i++) {
                    if (coord_ver[i] == 0 || coord_ver[i] == w - 1) {
                        arr[coord_hor[i]][coord_ver[i]] = 9;
                    }
                    for (j = 0; j < amount_dis; j++) {
                        /*if (coord_ver[i] == coord_ver[j]) {
                            if (coord_hor[i] - 1 == coord_hor[j] || coord_hor[i] + 1 == coord_hor[j]) {
                                arr[coord_hor[i]][coord_ver[i]] = 9;
                            }
                        }*/
                        if (coord_hor[i] == coord_hor[j]) {
                            if (coord_ver[i] - 1 == coord_ver[j] || coord_ver[i] + 1 == coord_ver[j]) {
                                arr[coord_hor[i]][coord_ver[i]] = 9;
                            }
                        }
                    }
                }

                // удаляю координаты остановившегося эл-та
                for (k = 0; k < amount_dis; k++) {
                    if (arr[coord_hor[k]][coord_ver[k]] == 9) {
                        for (j = k; j < amount_dis; j++) {
                            coord_hor[j] = coord_hor[j + 1];
                            coord_ver[j] = coord_ver[j + 1];
                        }
                        amount_dis -= 1;
                        k--;
                    }
                }

                // ПЕРЕМЕШИВАЮ КООРДИНАТЫ
                int coord_nums[amount_dis];
                for (i = 0; i < amount_dis; i++) {
                    coord_nums[i] = i;
                }


                for (int n = 0; n < amount_dis; ++n) {
                    swaparray(coord_nums, amount_dis, rnd_eng, forSwap);
                }
                // ДВИГАЮ ЕДИНИЦЫ
                for (k = 0; k < amount_dis; k++) {
                    if (maybe(0.5, rnd_eng, dstr)) { // вправо
                        if (arr[coord_hor[coord_nums[k]] + 1][coord_ver[coord_nums[k]]] != 1) {
                            arr[coord_hor[coord_nums[k]]][coord_ver[coord_nums[k]]] = 0;
                            arr[coord_hor[coord_nums[k]] + 1][coord_ver[coord_nums[k]]] = 1;
                            coord_hor[coord_nums[k]]++;

                        }
                    }
                    else { // влево
                        if (arr[coord_hor[coord_nums[k]] - 1][coord_ver[coord_nums[k]]] != 1) {
                            arr[coord_hor[coord_nums[k]]][coord_ver[coord_nums[k]]] = 0;
                            arr[coord_hor[coord_nums[k]] - 1][coord_ver[coord_nums[k]]] = 1;
                            coord_hor[coord_nums[k]]--;
                        }
                    }

                }
            }
            sum += counter;
        }
        cout << '\n' << "average= " << sum/40
             << '\n';
        arr_av[p]=sum/40;
    }

}

