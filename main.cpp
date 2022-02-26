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

void move(int type, int &coord_nums_k, int &coord_hor_k, int &coord_ver_k, int arr[][w],
              std::default_random_engine &rnd_eng, std::uniform_real_distribution<double> &dstr){

    if (arr[coord_hor_k][coord_ver_k] == type){
        if (maybe(0.5, rnd_eng, dstr)) { // По гориз
            if (maybe(0.5, rnd_eng, dstr)) { // вправо
                if (arr[coord_hor_k + 1][coord_ver_k] != type) {
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

int linear(int size, int (arr_av)[]);

using namespace std;

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
    if (var==1)
    {
        //int w = 10, h = 10;
        int arr[h][w]; // массив, представляющий кристалл
        int i, j;  // строчный и столбцовый индексы соотв.
        int amount_dis = 0;  // кол-во дислокаций

        for (int prob_dis = 50; prob_dis < 100; prob_dis++) {
            std::ofstream out;          // поток для записи
            int sum_steps = 0;
            for (int nom_exp = 0; nom_exp < 1; nom_exp++) {

                // заполняю клеточку кристалла атомом(0) или пустотой(1 || 2)
                for (i = 0; i < h; i++) {
                    for (j = 0; j < w; j++) {
                        if (maybe(prob_dis * 0.01, rnd_eng, dstr)){
                            arr[i][j] = maybe(0.5, rnd_eng, dstr)+1; // если 0, то +1 =1= красный; если 1, то +1=2=синий
                            amount_dis++;
                        }
                        else arr[i][j] = 0;

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
                        if (arr[i][j] == 1 || arr[i][j] == 2) {
                            coord_hor[k] = i;
                            coord_ver[k] = j;
                            k += 1;
                        }

                    }
                }
                cout << '\n';

                // ИЗОБРАЖАЮ МАССИВЫ КООРДИНАТ
                cout << '\n';
                for (i = 0; i <amount_dis; i++){
                    cout << coord_hor[i] << ' ';
                }
                cout << '\n';
                cout << '\n';
                for (i = 0; i <amount_dis; i++){
                    cout << coord_ver[i] << ' ';
                }
                cout << '\n';

                int counter = 0;

                while (amount_dis != 0) {
                    counter++;
                    // ПРОВЕРЯЮ НА СОСЕДСТВА ПУСТОТ, СОСЕДСТВА С ГРАНИЦЕЙ
                    for (i = 0; i < amount_dis; i++) {
                        if (coord_hor[i] == 0 || coord_hor[i] == 9 || coord_ver[i] == 0 || coord_ver[i] == 9) {
                            arr[coord_hor[i]][coord_ver[i]] = 9;
                        }
                        for (j = 0; j < amount_dis; j++) {
                            if (coord_ver[i] == coord_ver[j] && arr[coord_hor[i]][coord_ver[i]] == arr[coord_hor[j]][coord_ver[j]]) {
                                if (coord_hor[i] - 1 == coord_hor[j] || coord_hor[i] + 1 == coord_hor[j]) {
                                    arr[coord_hor[i]][coord_ver[i]] = 9;
                                }
                            }
                            if (coord_hor[i] == coord_hor[j] && arr[coord_hor[i]][coord_ver[i]] == arr[coord_hor[j]][coord_ver[j]]) {
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

                    // ИЗОБРАЖАЮ КРИСТАЛЛ СО СЛИПШИМИСЯ ПУСТОТАМИ
                    for (i = 0; i <= 9; i++) {
                        cout << '\n';
                        for (j = 0; j <= 9; j++) {
                            cout << arr[i][j] << ' ';
                        }
                    }
                    cout << '\n';

                    // ИЗОБРАЖАЮ МАССИВЫ КООРДИНАТ
                    /*cout << '\n';
                    for (i = 0; i < amount_dis; i++) {
                        cout << coord_hor[i] << ' ';
                    }
                    cout << '\n';
                    cout << '\n';
                    for (i = 0; i < amount_dis; i++) {
                        cout << coord_ver[i] << ' ';
                    }
                    cout << '\n';*/

                    // ПЕРЕМЕШИВАЮ КООРДИНАТЫ
                    int coord_nums[amount_dis];
                    for (i = 0; i < amount_dis; i++) {
                        coord_nums[i] = i;
                    }


                    for (int n = 0; n < amount_dis; ++n) {
                        swaparray(coord_nums, amount_dis, rnd_eng, forSwap);
                    }

                    // ДВИГАЮ ДИСЛОКАЦИИ
                    for (k = 0; k < amount_dis; k++) {

                        move(1, coord_nums[k], coord_hor[coord_nums[k]], coord_ver[coord_nums[k]], arr, rnd_eng, dstr);
                        move(2, coord_nums[k], coord_hor[coord_nums[k]], coord_ver[coord_nums[k]], arr, rnd_eng, dstr);

                    }

                    // ИЗОБРАЖАЮ КРИСТАЛЛ СО СЛИПШИМИСЯ ПУСТОТАМИ
                    /*for (i = 0; i <= 9; i++) {
                        cout << '\n';
                        for (j = 0; j <= 9; j++) {
                            cout << arr[i][j] << ' ';
                        }
                    }
                    cout << '\n';*/
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
