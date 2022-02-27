#include <iostream>
#include <fstream>
#include <random> //random engine, random distribution
#include <chrono>


const int w = 10;
const int h = 10;

bool maybe(double prob, 
    std::default_random_engine &rnd_eng,
    std::uniform_real_distribution<double> &dstr) {
   return dstr(rnd_eng) <= prob;
}

void swaparray(int m[], int size,
               std::default_random_engine &rnd_eng,
               std::uniform_int_distribution<size_t> &UID) {
    /*m: название массива
    size: кол-во элементов
    rnd_eng: генератор случайных чисел
    UID: нормальное целочисленное распределение*/
    for (size_t i = 0; i < size; ++i) {
        std::swap(m[i], m[UID(rnd_eng) % size]);
    }
}

void move(int type,  int &coord_hor_k, int &coord_ver_k, int arr[][w],
              std::default_random_engine &rnd_eng, std::uniform_real_distribution<double> &dstr){

    if (arr[coord_hor_k][coord_ver_k] == type){
        if (maybe(0.5, rnd_eng, dstr)) { // По гориз
            if (maybe(0.5, rnd_eng, dstr)) { // вправо
                if (arr[coord_hor_k + 1][coord_ver_k] != type ) {
                    arr[coord_hor_k][coord_ver_k] = 0;
                    arr[coord_hor_k + 1][coord_ver_k] = type;
                    coord_hor_k++;

                }
            } else { // влево
                if (arr[coord_hor_k - 1][coord_ver_k] != type) {
                    arr[coord_hor_k][coord_ver_k] = 0;
                    arr[coord_hor_k - 1][coord_ver_k] = type;
                    coord_hor_k--;
                }
            }
        } else { // По вертик
            if (maybe(0.5, rnd_eng, dstr)) { // вверх
                if (arr[coord_hor_k][coord_ver_k + 1] != type) {
                    arr[coord_hor_k][coord_ver_k] = 0;
                    arr[coord_hor_k][coord_ver_k + 1] = type;
                    coord_ver_k++;
                }
            } else { // вниз
                if (arr[coord_hor_k][coord_ver_k - 1] != type) {
                    arr[coord_hor_k][coord_ver_k] = 0;
                    arr[coord_hor_k][coord_ver_k - 1] = type;
                    coord_ver_k--;
                }
            }
        }
    }
}

using namespace std;

// ИЗОБРАЖАЮ МАССИВЫ КООРДИНАТ
void show_coord(int amount_dis,int coord_hor[], int coord_ver[]){
    cout << '\n';
    for (int k = 0; k < amount_dis; k++) {
        cout << coord_hor[k] << ' ';
    }
    cout << '\n';
    cout << '\n';
    for (int k = 0; k < amount_dis; k++) {
        cout << coord_ver[k] << ' ';
    }
    cout << '\n';
}

// ИЗОБРАЖАЮ КРИСТАЛЛ СО СЛИПШИМИСЯ ПУСТОТАМИ
void show_crystal(int arr[][w]) {
    for (int i = 0; i < h; i++) {
        cout << '\n';
        for (int j = 0; j < w; j++) {
            cout << arr[i][j] << ' ';
        }
    }
    cout << '\n';
}

// ПРОВЕРЯЮ НА СОСЕДСТВА ПУСТОТ, СОСЕДСТВА С ГРАНИЦЕЙ
void delete_9_8(int &i, int &amount_dis, int coord_hor[], int coord_ver[]){
    int del_idx;
    for (del_idx = i; del_idx<amount_dis; del_idx++) {
        coord_hor[del_idx] = coord_hor[del_idx + 1];
        coord_ver[del_idx] = coord_ver[del_idx + 1];
    }
    amount_dis -= 1;
    i--;
}
void in_crystal(int j_start, int j_stop, int i, int type, int coord_hor[], int coord_ver[], int arr[][w]) {
    int j;
    for (j = j_start; j < j_stop; j++) {
        if (coord_ver[i] == coord_ver[j]){
            if (type == arr[coord_hor[j]][coord_ver[j]]) {
                if (coord_hor[i] - 1 == coord_hor[j] || coord_hor[i] + 1 == coord_hor[j]) {
                    arr[coord_hor[i]][coord_ver[i]] = 10 - type;
                    arr[coord_hor[j]][coord_ver[j]] = 10 - type;
                    cout << "hor " <<coord_hor[i]<< "ver " << coord_ver[i];
                    //delete_9_8(i, amount_dis, coord_hor, coord_ver);
                }
            }
            else if (10 - type == arr[coord_hor[j]][coord_ver[j]]) {
                if (coord_hor[i] - 1 == coord_hor[j] || coord_hor[i] + 1 == coord_hor[j]) {
                    arr[coord_hor[i]][coord_ver[i]] = 10 - type;
                    cout << "hor " <<coord_hor[i]<< "ver " << coord_ver[i];
                }
            }

        }

        if (coord_hor[i] == coord_hor[j]) {
            if (type == arr[coord_hor[j]][coord_ver[j]]) {
                if (coord_ver[i] - 1 == coord_ver[j] || coord_ver[i] + 1 == coord_ver[j]) {
                    arr[coord_hor[i]][coord_ver[i]] = 10 - type;
                    arr[coord_hor[j]][coord_ver[j]] = 10 - type;
                    //delete_9_8(i, amount_dis, coord_hor, coord_ver);
                    cout << "hor " <<coord_hor[i]<< "ver " << coord_ver[i];
                }
            }
            else if (10 - type == arr[coord_hor[j]][coord_ver[j]]) {
                if (coord_ver[i] - 1 == coord_ver[j] || coord_ver[i] + 1 == coord_ver[j]) {
                    arr[coord_hor[i]][coord_ver[i]] = 10 - type;
                    cout << "hor " <<coord_hor[i]<< "ver " << coord_ver[i];
                }
            }
        }
    }
}
void check(int type, int &amount_dis, int amount_moving, int coord_hor[], int coord_ver[],int coord_hor_m[], int coord_ver_m[], int arr[][w]) {
    int i;
    for (i = 0; i < amount_moving; i++) {
        if (arr[coord_hor_m[i]][coord_ver_m[i]] == type) {
            if (coord_hor_m[i] == 0 || coord_hor_m[i] == 9 || coord_ver_m[i] == 0 || coord_ver_m[i] == 9
                    ) {
                arr[coord_hor_m[i]][coord_ver_m[i]] =
                        10 - type; // 1-> 9, а 2-> 8. FIXME: Чтобы 1 не взаим. с 8, а 2 - с 9.
                //delete_9_8(i, amount_dis, coord_hor, coord_ver);
            }
        }
    }
    for (i = 0; i < amount_dis; i++) {
        if (arr[coord_hor[i]][coord_ver[i]] == type){
            in_crystal(0,i, i, type,  coord_hor, coord_ver, arr);
            //Избегаю проверять дислокацию с самой собой, то есть когда i == j, т.к. это бесполезно
            in_crystal(i+1,amount_dis, i, type, coord_hor, coord_ver, arr);
        }
    }
}

// Показываю, что есть во втором файле данная функция, отвечающая за случай 1-мерного кристалла
int linear(int size, int (arr_av)[]);


int main() {
    auto current_time =
        std::chrono::system_clock::now().time_since_epoch();
    auto seed =
        std::chrono::duration_cast<std::chrono::seconds>(current_time).count();

    std::default_random_engine rnd_eng(seed);
    std::uniform_real_distribution<double> dstr(0.0,1.0);
    std::uniform_int_distribution<size_t> forSwap(0,1000000);

    cout << "Write 1(works) or 2(doesn't work) " << '\n';
    int var; // отвечает за одномерный или многомерный случай
    cin >> var ;
    if (var==1){

        //int w = 10, h = 10;
        int arr[h][w]; // массив, представляющий кристалл
        int i, j;  // строчный и столбцовый индексы соотв.
        int amount_dis = 0;  // кол-во дислокаций в момент генераци
        int amount_moving = 0;

        for (int prob_dis = 50; prob_dis < 100; prob_dis++) {
            std::ofstream out;          // поток для записи
            int sum_steps = 0;
            for (int nom_exp = 0; nom_exp < 1; nom_exp++) {

                // заполняю клеточку кристалла атомом(0) или пустотой(1 || 2)
                for (i = 0; i < h; i++) {
                    for (j = 0; j < w; j++) {
                        if (maybe(prob_dis * 0.01, rnd_eng, dstr)){
                            arr[i][j] = maybe(0.5, rnd_eng, dstr)+1; // если 0, то +1 =1=красный; если 1, то +1=2=синий; их вер-ть равна
                            amount_dis++;
                            amount_moving++;
                        }
                        else arr[i][j] = 0;

                        //cout << arr[i][j] << ' ';
                    }
                    //cout << '\n';
                }
                //cout << '\n';

                // Массивы с последними координатами всех когда-либо движимых дислокаций
                int coord_hor[amount_dis]; // гориз коорд dislocation
                int coord_ver[amount_dis]; // с вертикальными коорд.
                //  Массивы с теми же коорд., но будут уменьшаться, так как содержат только ныне Moving dislocation
                int coord_hor_m[amount_moving];
                int coord_ver_m[amount_moving];


                // заполняю массивы координат единиц
                int k = 0; // индекс эл-ов массивов координат
                for (j = 0; j < w; j++) {
                    for (i = 0; i < h; i++) {
                        if (arr[i][j] == 1 || arr[i][j] == 2) {
                            coord_hor[k] = i;
                            coord_ver[k] = j;
                            coord_hor_m[k] = i;
                            coord_ver_m[k] = j;
                            k++;
                        }
                    }
                }
                cout << '\n';

                show_coord(amount_moving, coord_hor_m, coord_ver_m);

                int counter = 0; // счётчик кол-ва шагов


                while (amount_moving != 0) {
                    cout << "-----";
                    show_crystal(arr);
                    counter++;
                    check(2, amount_dis,amount_moving, coord_hor, coord_ver, coord_hor_m, coord_ver_m, arr);
                    check(1, amount_dis,amount_moving, coord_hor, coord_ver, coord_hor_m, coord_ver_m, arr);

                    // удаляю координаты остановившегося эл-та
                    for (k = 0; k < amount_moving; k++) {
                        if (arr[coord_hor_m[k]][coord_ver_m[k]] == 8 ||arr[coord_hor_m[k]][coord_ver_m[k]] == 9) {
                            for (j = k; j < amount_moving; j++) {
                                coord_hor_m[j] = coord_hor_m[j + 1];
                                coord_ver_m[j] = coord_ver_m[j + 1];
                            }
                            amount_moving -= 1;
                            k--;
                        }
                    }
                    show_coord(amount_moving, coord_hor_m, coord_ver_m);
                    show_crystal(arr);
                    show_coord(amount_moving, coord_hor_m, coord_ver_m);
                    show_coord(amount_dis, coord_hor, coord_ver);

                    // ПЕРЕМЕШИВАЮ КООРДИНАТЫ
                    int coord_nums[amount_moving];
                    for (i = 0; i < amount_moving; i++) {
                        coord_nums[i] = i;
                    }

                    for (int n = 0; n < amount_dis; ++n) {
                        swaparray(coord_nums, amount_moving, rnd_eng, forSwap);
                    }

                    // ДВИГАЮ ДИСЛОКАЦИИ
                    for (k = 0; k < amount_moving; k++) {
                        move(1,coord_hor_m[coord_nums[k]], coord_ver_m[coord_nums[k]], arr, rnd_eng, dstr);
                        move(2,coord_hor_m[coord_nums[k]], coord_ver_m[coord_nums[k]], arr, rnd_eng, dstr);
                    }


                }

                /*cout << '\n' << "counter= " << counter
                     << '\n'; // -1 т.к. последний ход нужен только, чтобы зафикс. единицу как 9*/

                /*out.open(
                        "C:\\Users\\пк\\Desktop\\CLionProjects\\Лаба1\\Многомерный массив.txt"); // окрываем файл для записи
                if (out.is_open()) {
                    out << counter << ' ';
                }*/
                sum_steps += counter;
            }
            /*out.open(
                    "C:\\Users\\пк\\Desktop\\CLionProjects\\Лаба1\\Многомерный массив.txt"); // окрываем файл для записи
            if (out.is_open()) {
                out << '\n' << std::endl;
            }*/
            cout << '\n' << "average= " << sum_steps / 40
                 << '\n';
        }
    }
    else if (var==2){
        int arr_av[100];
        linear(100, arr_av);
    }
    return 0;
}
