#include <iostream>
#include <random>

using i64 = int64_t;
using u64 = uint64_t;

void MergeSortInterval(i64* a, i64* b, size_t n);
void PrintArray(i64* arr, size_t n);

void MergeSort(i64* arr, size_t n) {
    i64* a = arr;
    i64* b = new i64[n];

    MergeSortInterval(a, b, n);
}

void Merge(i64* a1, size_t n1, i64* a2, size_t n2, i64* b) {
    u64 i1 = 0, i2 = 0, ib = 0;

    while ((i1 < n1) && (i2 < n2)) {
        if (a1[i1] < a2[i2]) {
            b[ib] = a1[i1];
            i1++;
        } else {
            b[ib] = a2[i2];
            i2++;
        }
        ib++;
    }

    while (i1 < n1) {
        b[ib] = a1[i1];
        i1++;
        ib++;
    }

    while (i2 < n2) {
        b[ib] = a2[i2];
        i2++;
        ib++;
    }
}

void MergeSortInterval(i64* a, i64* b, size_t n) {
    if (n < 1) {
        return;
    }

    if (n == 1) {
        *b = *a; // ~ b[0] = a[0]
        return;
    }

    if (n == 2) {
        if (a[0] < a[1]) {
            return;
        } else {
            b[0] = a[1];
            b[1] = a[0];
        }
    }

    // Sort part
    size_t n_l = n / 2;
    size_t n_r = n - n_l;
    MergeSortInterval(a, b, n_l);
    MergeSortInterval(a + n_l, b + n_l, n_r);

    // Merge parts
    Merge(a, n_l, a + n_l, n_r, b);

    for (i64 i = 0; i < n; i++) {
        a[i] = b[i];
    }
}

void PrintArray(i64* arr, size_t n) {
    for (u64 i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}


bool BinSearchRec(i64* arr, size_t n, i64 needle) {
    if (n == 0) {
        return false;
    }
    if (n == 1) {
        if (arr[0] == needle) {
            return true;
        } else {
            return false;
        }
    }

    if (arr[n / 2] > needle) {
        return BinSearchRec(arr, n / 2, needle);
    } else if (arr[n / 2] < needle) {
        return BinSearchRec(arr + n / 2, n - n / 2, needle);
    } else {
        return true;
    }
}


bool BinSearch(i64* arr, size_t n, i64 needle) {
    i64* b = arr;
    size_t m = n;
    while (m != 0 || m != 1) {
        if (b[m / 2] < needle) {
            m /= 2;
        } else if (b[m / 2] > needle) {
            b += m / 2;
            m -= m / 2;
        } else {
            return true;
        }
        std::cout << "ok\n";
    }
    
    if (m == 0) {
        return false;
    } 
    if (m == 1) {
        if (b[0] == needle) {
            return true;
        } else {
            return false;
        }
    }
}


int main() {
    size_t n;
    std::cin >> n;
    i64* arr = new i64[n];

    // Случайная генерация массива
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<i64> dist(0, n);

    for (u64 i = 0; i < n; i++) {
        // std::cin >> arr[i];
        i64 rn = dist(gen);
        arr[i] = rn;
        std::cout << rn << " ";
    }
    std::cout << std::endl;

    // Сортировка и поиск элемента
    MergeSort(arr, n);
    PrintArray(arr, n);
    std::cout << "Ищем 5: ";
    std::cout << std::boolalpha << BinSearchRec(arr, n, 5) << std::endl;
    std::cout << std::boolalpha << BinSearch(arr, n, 5) << std::endl;

    delete[] arr;
    return 0;
}