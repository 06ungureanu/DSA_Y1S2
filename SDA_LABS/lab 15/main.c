#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INVALID_INPUT   "Invalid input\n"
#define SUCCES_CODE     0
#define FAILURE_CODE    -1     

typedef struct Student {
    char *name;
    float grade;
} Student;

typedef struct Restaurant {
    int id;
    int rating;
    int veganFriendly;
    int price;
    int distance;
} Restaurant;

static inline bool _check_int_array(int *v, int size) {
    
    return v && size > 0;
}
static inline int min(int a, int b) {
    return (a < b) ? a : b;
}
int cmp_int(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}
int cmp_chr(const void *a,const void *b) {
    return (*(char *)b - *(char *)a);
}
int cmp_std(const void *a, const void *b) {
    return ((Student*)b)->grade - ((Student*)a)->grade;
}
void swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}
void merge(int *v, int low, int mid, int high) {
    
    int *temp = (int*)malloc(sizeof(int) * (high - low + 1));

    

    int i = low;        
    int j = mid + 1;    
    int k = 0;
    
    
    while(i <= mid && j <= high) {
        if(v[i] <= v[j])    
            temp[k++] = v[i++];
        else                
            temp[k++] = v[j++];
    }

    
    while(i <= mid)
        temp[k++] = v[i++];
    
    while(j <= high)
        temp[k++] = v[j++];
    
    
    for(int i = low; i <= high; i++)
        v[i] = temp[i - low];
    
    
    free(temp);
}
void merge_sort_recursiv(int *v, int low, int high) {
    if(low >= high) return;

    int mid = (low +  high) / 2;

    merge_sort_recursiv(v,low, mid);        
    merge_sort_recursiv(v,mid + 1, high);   
    merge(v,low,mid,high);                  
}
void merge_sort_iterative(int *v, int size) {
    if(!v || size < 2) return;

    for(int curr = 1; curr <= size - 1; curr *= 2) {
        for(int left = 0; left < size - 1; left += curr * 2) {
            int mid = min(left + curr - 1, size - 1);
            int right = min(left + 2 * curr - 1, size - 1);

            merge(v,left,mid,right);
        }
    }
}
int median_of_three(int *v, int a, int b, int c) {
    if ((v[a] < v[b] && v[b] < v[c]) || (v[c] < v[b] && v[b] < v[a]))
        return b;
    if ((v[b] < v[a] && v[a] < v[c]) || (v[c] < v[a] && v[a] < v[b]))
        return a;
    return c;
}
int partition(int *v, int low, int high) {
    int partition_index = low + rand() % (high - low + 1);
    int pivot = v[partition_index];

    int i = low;
    int j = high;

    while (i <= j) {
        while (v[i] < pivot) i++;
        while (v[j] > pivot) j--;

        if (i <= j) {
            swap(&v[i], &v[j]); 
            i++;
            j--;
        }
    }

    return i; 
}
void my_quick_sort(int *v, int low, int high) {
    if(low < high) {
        int partition_index = partition(v,low,high);
        
        my_quick_sort(v,low, partition_index - 1);
        my_quick_sort(v,partition_index, high);
    }
}
void counting_sort(int *v, int size, int exp) {
    int *output = (int*)calloc(size, sizeof(int));
    int count[10] = {0};

    
    for(int i = 0; i < size; i++) {
        int digit = (v[i] / exp) % 10;
        count[digit]++;
    }
    
    
    for(int i = 1; i < 10; i++)
        count[i] += count[i - 1];
    
    
    for(int i = size - 1; i >= 0; i--) {
        int digit = (v[i] / exp) % 10;
        output[count[digit] - 1] = v[i];
        count[digit]--;
    }

    for(int i = 0; i < size; i++)
        v[i] = output[i];
    
    
    free(output);
}
void radix_sort(int *v, int size) {
    int max = v[0];
    for(int i = 0; i < size; i++)
        if(v[i] > max)
            max = v[i];
        
    for(int exp = 1; max / exp > 0; max *= 10)
        counting_sort(v,size,exp);
}
void cookie_problem() {
   int g[] = {1,2,3};
   int s[] = {1,2};

   int g_size = sizeof(g) / sizeof(g[0]);
   int s_size = sizeof(s) / sizeof(s[0]);

   radix_sort(s,s_size);
   radix_sort(g,g_size);

    int i = 0;
    int j = 0;
    int happy_child = 0;

    while(i <= g_size && j <= s_size) {
        if(s[i] >= g[i]) happy_child++;
        i++;
        j++;
    }

    printf("Happy childs : %d \n", happy_child);
}
int count_vegan_friendly(Restaurant *res, int n) {
    if(!res) return -1;

    int friendly = 0;
    for(int i = 0; i < n; i++)
        if(res[i].veganFriendly == 1) friendly++;

    return friendly;
}
void print_restaurant_list(Restaurant *rest, int n) {
    if(!rest || n < 1) return;

    printf("[ ");
    for(int i = 0; i < n; i++)
        printf("%d ", rest[i].id);
    printf("]\n");
}
int compare_restaurante(Restaurant a, Restaurant b) {
    if(a.rating != b.rating)
        return b.rating - a.rating;
    return b.id - a.id;
}
void merge_restaurante(Restaurant *v, int low, int mid, int high) {
    Restaurant *temp = (Restaurant*)malloc((high - low + 1) * sizeof(Restaurant));

    int i = low;        
    int j = mid + 1;    
    int k = 0;          

    while(i <= mid && j <= high) {
        if(compare_restaurante(v[i],v[j]) <= 0)
            temp[k++] = v[i++];
        else
            temp[k++] = v[j++];
    }

    while(i <= mid) temp[k++] = v[i++];
    while(j <= high) temp[k++] = v[j++];

    for(int i = low; i <= high; i++)
        v[i] = temp[i - low];

    free(temp);
}
void merge_sort_restaurant(Restaurant *v, int low, int high) {
    if(low >= high) return;

    int mid = low + (high - low) / 2;
    merge_sort_restaurant(v,low,mid);
    merge_sort_restaurant(v, mid + 1,high);
    merge_restaurante(v,low,mid,high);
}
void show_great_restaurants(Restaurant *res, int n, int vegan_friendly, int max_price, int max_distance) {
    if(!res || n < 1) return;

    Restaurant *filtred = (Restaurant*)malloc(n * sizeof(Restaurant));
    int size = 0;

    for(int i = 0; i < n; i++) {
        
        if(res[i].price <= max_price && res[i].distance <= max_distance)  
            if(vegan_friendly == 0 || res[i].veganFriendly == 1)
                filtred[size++] = res[i];
    }

    merge_sort_restaurant(filtred,0,size - 1);
    print_restaurant_list(filtred,size);
    free(filtred);
}
void print_array(int *v, int size) {
    if(!_check_int_array(v,size)) {
        printf(INVALID_INPUT);
        return;
    }

    printf("[ ");
    for(int i = 0; i < size; i++) 
        printf("%d ", v[i]);   
    
    printf("]\n");
}
void print_char_array(char *v, int size) {
    printf("[ ");
    for(int i = 0; i < size; i++)
        printf("%c ",v[i]);
    printf("]\n");
}
void print_stud_array(Student *studs, int size) {
    printf("[ ");
    for(int i = 0; i < size; i++) 
        printf("%s %.1f, ", studs[i].name,studs[i].grade);
    printf("]\n");
}
int main() {

    int v[] = {5,3,7,2,1,8,6,9,4};
    char c[] = {'a','b','c','d','e','f'};
    Student studs[] = {{"Cristi",3.4},{"Mirela",3,5}, {"Alex",9.7},{"Denis",1.2}};
    Restaurant rest[] = {{1,4,1,40,10},{2,8,0,50,5},{3,8,1,30,4},{4,10,0,10,3},{5,1,1,15,1}};


    int s_size = sizeof(studs) / sizeof(studs[0]);
    int v_size = sizeof(v) / sizeof(v[0]);
    int c_size = sizeof(c) / sizeof(c[0]);
    int total_rest = sizeof(rest) / sizeof(rest[0]);

    qsort(c,c_size,sizeof(char),cmp_chr);
    qsort(studs,s_size,sizeof(Student),cmp_std);
    radix_sort(v,v_size);

    print_array(v,v_size);
    print_char_array(c,c_size);
    print_stud_array(studs,s_size);
    cookie_problem();
    show_great_restaurants(rest,total_rest,1,50,10);
    
    return 0;
}