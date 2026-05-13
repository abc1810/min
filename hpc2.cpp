#include <iostream>
#include <omp.h>
using namespace std;

// ---------------- Bubble Sort ----------------

// Sequential Bubble Sort
void seqBubble(int a[], int n) {
    for(int i=0;i<n-1;i++) {
        for(int j=0;j<n-i-1;j++) {
            if(a[j] > a[j+1])
                swap(a[j], a[j+1]);
        }
    }
}

// Parallel Bubble Sort
void parBubble(int a[], int n) {
    for(int i=0;i<n-1;i++) {

        #pragma omp parallel for
        for(int j=0;j<n-i-1;j++) {
            if(a[j] > a[j+1])
                swap(a[j], a[j+1]);
        }
    }
}

// ---------------- Merge Sort ----------------

// Merge Function
void merge(int a[], int l, int m, int r) {

    int n1 = m-l+1;
    int n2 = r-m;

    int L[n1], R[n2];

    for(int i=0;i<n1;i++)
        L[i] = a[l+i];

    for(int i=0;i<n2;i++)
        R[i] = a[m+1+i];

    int i=0, j=0, k=l;

    while(i<n1 && j<n2) {

        if(L[i] < R[j])
            a[k++] = L[i++];
        else
            a[k++] = R[j++];
    }

    while(i<n1)
        a[k++] = L[i++];

    while(j<n2)
        a[k++] = R[j++];
}

// Sequential Merge Sort
void seqMerge(int a[], int l, int r) {

    if(l < r) {

        int m = (l+r)/2;

        seqMerge(a, l, m);
        seqMerge(a, m+1, r);

        merge(a, l, m, r);
    }
}

// Parallel Merge Sort
void parMerge(int a[], int l, int r) {

    if(l < r) {

        int m = (l+r)/2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parMerge(a, l, m);

            #pragma omp section
            parMerge(a, m+1, r);
        }

        merge(a, l, m, r);
    }
}

// ---------------- Main ----------------

int main() {

    int n;

    cout<<"Enter number of elements: ";
    cin>>n;

    int a[n], b[n], c[n], d[n];

    cout<<"Enter elements:\n";

    for(int i=0;i<n;i++) {

        cin>>a[i];

        b[i]=a[i];
        c[i]=a[i];
        d[i]=a[i];
    }

    double start, end;

    // Sequential Bubble Sort
    start = omp_get_wtime();

    seqBubble(a, n);

    end = omp_get_wtime();

    cout<<"\nSequential Bubble Sort:\n";

    for(int i=0;i<n;i++)
        cout<<a[i]<<" ";

    cout<<"\nTime = "<<end-start<<" sec\n";

    // Parallel Bubble Sort
    start = omp_get_wtime();

    parBubble(b, n);

    end = omp_get_wtime();

    cout<<"\nParallel Bubble Sort:\n";

    for(int i=0;i<n;i++)
        cout<<b[i]<<" ";

    cout<<"\nTime = "<<end-start<<" sec\n";

    // Sequential Merge Sort
    start = omp_get_wtime();

    seqMerge(c, 0, n-1);

    end = omp_get_wtime();

    cout<<"\nSequential Merge Sort:\n";

    for(int i=0;i<n;i++)
        cout<<c[i]<<" ";

    cout<<"\nTime = "<<end-start<<" sec\n";

    // Parallel Merge Sort
    start = omp_get_wtime();

    parMerge(d, 0, n-1);

    end = omp_get_wtime();

    cout<<"\nParallel Merge Sort:\n";

    for(int i=0;i<n;i++)
        cout<<d[i]<<" ";

    cout<<"\nTime = "<<end-start<<" sec";

    return 0;
}