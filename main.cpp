#include <iostream>
 #include <vector>
 #include <thread>
 #include <algorithm>
 #include <functional>

 bool cmp(const int& a, const int& b) {
     //return b > a;
     return b < a;
}

template <typename T, typename T2>
void merge_sort(std::vector<T>& input, int *threads_count, int max_threads, T2 comp) {
    if(input.size() < 2) return; // если массив из одного элемента, то он отсортирован
    int mid = input.size() / 2; // ищем середину
    std::vector<T> left(mid); // создаем левый вектор
    std::copy(input.begin(), input.begin() + mid, left.begin()); // заполняем левый вектор

    std::vector<T> right(input.size() - mid);// создаем правый вектор
    std::copy(input.begin() + mid, input.end(), right.begin()); //заполняем правый вектор

    if((*threads_count < max_threads - 1) && input.size() > 1000){ // если
        *threads_count+=1; // прибавим поток
        std::thread thl(merge_sort<T, T2>, std::ref(left), threads_count, max_threads, comp);
        //создаем поток 1-,2-,3-,4-,5-.
        merge_sort(right, threads_count, max_threads, comp);
        thl.join();
        *threads_count-=1;
    }
    else { // если
        merge_sort(left, threads_count, max_threads, comp);
        merge_sort(right, threads_count, max_threads, comp);
    }
    std::merge(left.begin(), left.end(), right.begin(), right.end(), input.begin(), comp); // собирает input обратно согласно comp

}

int main() {
    srand(time(nullptr));
    unsigned int c = std::thread::hardware_concurrency();
    std::cout << "number of cores: " << c << std::endl;;
    std::vector<int> array1;
    std::vector<int> array2;

    for(int i = 0; i < 100000; ++i) {
        array1.push_back(rand() % 10000);
        array2.push_back(array1.back());
    }

    std::cout << "-----Unsorted array-----\n";
    for(const auto &item : array1) {
        std::cout << item << " ";
    }

    int* threads_count = new int(0); // зачем?

    auto start = std::chrono::system_clock::now();
    merge_sort(array1, threads_count, c/2, cmp); // почему c/2? разве не 8
    auto end = std::chrono::system_clock::now();
    auto elapsed_multicore = end - start;

    start = std::chrono::system_clock::now();
    merge_sort(array2, threads_count, 1, cmp);
    end = std::chrono::system_clock::now();
    auto elapsed_singlecore = end - start;

    std::cout << "\n-----Sorted array-----\n";
    for(const auto &item : array1) {
        std::cout << item << " ";
    }

    std::cout << "\nmulticore time:\n" << elapsed_multicore.count() << " ms" << '\n';

    std::cout << "singlecore time:\n" << elapsed_singlecore.count() << " ms" << '\n';

    return 0;
}