#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>


using namespace std;
using namespace chrono;


class piece { // used to slice file to K
  public : int start, mid, end;

  public : piece(int a, int b) {
    start = a, end = b, mid = 0;
  } // constructor
}; // class piece

struct bubbleArgs {
  int *arr;
  int start,end;
}; // struct bubbleArgs

struct mergeArgs {
  int *arr;
  int start, mid, end;
}; // struct mergeArgs


void printMenu(char** filename, int& k, int& method);
void printTime(milliseconds duration);
void readFile(vector<int>& vec, ifstream& file);
char* StringTransToCharPointer(string s);
void sliceToK(vector<piece*>& pieces, int size, int k);

void bubbleSort(int* arr, int start, int end);
void *bubbleSort(void *args);
void mergeSort(int* arr, int left, int mid, int right);
void *mergeSort(void *args);

void method_1(int* arr, int size, int k);
void method_2(int* arr, int size, int k);
void method_3(int* arr, int size, int k);
void method_4(int* arr, int size, int k);


int main() {

  while(true) {
    char* fileName;
    int k, method;
    ifstream file;

    do {
      printMenu(&fileName, k, method);
      file.open(fileName); // try to open file

     if (method == -1 || !file) cout << "Input error, please try again.\n\n";
    } while(method == -1 || !file);

    vector<int> vec;
    readFile(vec, file); // Read data from file
    int size = vec.size();
    int *arr = new int[size];  // allocate array

    copy(vec.begin(), vec.end(), arr);
    vec.clear();

    if      (method == 1) method_1(arr, size, k);
    else if (method == 2) method_2(arr, size, k);
    else if (method == 3) method_3(arr, size, k);
    else if (method == 4) method_4(arr, size, k);

    delete[] arr;
  } // while(true)

} // main()


void printMenu(char** fileName, int& k, int& method) {
  string s;
  bool allIsDigit = true;

  cout << "Please enter the file name : ";
  getline(cin, s);
  s = s + ".txt";
  *fileName = StringTransToCharPointer(s);

  cout << "Please enter the number of partitions : ";
  getline(cin, s);

  k = 0;
  for (int a = 0; a < s.length(); a++) { // check input K
    if (!isdigit(s[a])) allIsDigit = false;
    else k = k * 10 + (s[a] - '0');
  } // for(a)

  cout << "Please enter the method No.(1, 2, 3, 4) : ";
  getline(cin, s);

	if (!isdigit(s[0]) || s.length() > 1) allIsDigit = false;
	else method = (s[0] - '0');

  if (!allIsDigit) method = -1; // means input error
} // printMenu()


void readFile(vector<int>& vec, ifstream& file) {
    int temp; // Read data and store it in a vector
    while(file >> temp) vec.push_back(temp);

    file.close(); // close file
} // readFile()


char* StringTransToCharPointer(string s) {
  char* fileName = new char[s.length() + 1]; // string transfer to char[]
  for (int a = 0; a < s.length(); a++) fileName[a] = s[a];
  fileName[s.length()] = '\0';
  return fileName;
} // StringTransToCharStar()


void sliceToK(vector<piece*>& pieces, int size, int k) {
  int pieceCapacity = size / k;
  int odd           = size % k;

  for (int a = 0; a < k; a++) {
    int start = a       * pieceCapacity;
    int end   = (a + 1) * pieceCapacity - 1;

    if (end > size)
      end = size;

    piece* p = new piece(start, end);
    pieces.push_back(p);
  } // for(a)

  if (odd > 0) {
    for (int a = 0; a < pieces.size(); a++) {
      if (a == 0)
        pieces[0]->end = pieces[0]->end + 1;
      else if (a < odd)
        pieces[a]->start = pieces[a]->start + a,   pieces[a]->end = pieces[a]->end + a + 1;
      else
        pieces[a]->start = pieces[a]->start + odd, pieces[a]->end = pieces[a]->end + odd;
    } // for(a)
  } // if()

} // sliceToK()


void bubbleSort(int* arr, int start, int end) {
  for (int a = start; a < end; a++)
    for (int b = a + 1; b <= end; b++)
      if (arr[a] > arr[b])
        swap(arr[a], arr[b]);
} // bubbleSort()


void *bubbleSort(void *args) {
  bubbleArgs *ba = (bubbleArgs*)args;
  bubbleSort(ba->arr, ba->start, ba->end);
  pthread_exit( 0 );
} // *bubbleSort()


void mergeSort(int* arr, int left, int mid, int right) {
  int i = left, j = mid, k = 0;
  vector<int> temp(right - left + 1); // Merge left and right subarrays

  while (i < mid && j <= right) {
    if (arr[i] < arr[j]) temp[k++] = arr[i++];
    else                 temp[k++] = arr[j++];
  } // while()

  while (i <  mid)   temp[k++] = arr[i++];
  while (j <= right) temp[k++] = arr[j++];

  for (i = left, k = 0; i <= right; i++) // Copy back to the original vector
    arr[i] = temp[k++];
} // mergeSort()


void *mergeSort(void *args) {
  mergeArgs *ma = (mergeArgs*)args;
  mergeSort(ma->arr, ma->start, ma->mid, ma->end);
  pthread_exit( 0 );
} // *bubbleSort()


void method_1(int* arr, int size, int k) {
  auto start = high_resolution_clock::now(); // Start timing

  bubbleSort(arr, 0, size - 1); // Execute bubbleSort

  auto end = high_resolution_clock::now(); // Stop timing
  auto duration = duration_cast<milliseconds>(end - start); // Calculate execution time in milliseconds

  //for (int i = 0; i < size; i++) cout << arr[i] << "\n";
  printTime(duration);
} // method_1()


void method_2(int* arr, int size, int k) {
  auto start = high_resolution_clock::now(); // Start timing
  vector<piece*> pieces; // Split the array into pieces, where piece is a struct
  sliceToK(pieces, size, k); // containing the start and end positions of the block

  for (int a = 0; a < k; a++) // Perform bubble sort on each data segment
    bubbleSort(arr, pieces[a]->start, pieces[a]->end);

  while (pieces.size() > 1) { // Create threads and execute mergeSort
    int a = 0;
    if (pieces.size() % 2) pieces[pieces.size() - 1]->mid = 0; // to mark the lonely block

    while(a + 1 < pieces.size()) {
      pieces[a]->mid = pieces[a + 1]->start;
      pieces[a]->end = pieces[a + 1]->end;
      pieces.erase(pieces.begin() + ++a);
    } // while()

    for (int b = 0; b < pieces.size(); b++)
      if (pieces[b]->mid) // We need to merge two blocks, don't merge the lonely block
        mergeSort(arr, pieces[b]->start, pieces[b]->mid, pieces[b]->end);
  } // while()

  pieces.clear();

  auto end = high_resolution_clock::now(); // Stop timing
  auto duration = duration_cast<milliseconds>(end - start); // Calculate execution time in milliseconds

  //for (int i = 0; i < size; i++) cout << arr[i] << "\n";
  printTime(duration);
} // method_2()


void method_3(int* arr, int size, int k) {
  auto start = high_resolution_clock::now(); // Start timing

  int shmid = shmget(IPC_PRIVATE, size* sizeof(int), IPC_CREAT | 0600);
  int *shmaddr = NULL; // Later, a virtual space will be created for sorting.
  vector<pid_t>  pids;
  vector<piece*> pieces; // Split the array into pieces, where piece is a struct
  sliceToK(pieces, size, k); // containing the start and end positions of the block

  for (int a = 0; a < k; a++) { // Perform bubble sort on each data segment
    pid_t pid = fork();

    if (pid == -1) { // error
      perror("fork error");
      exit( EXIT_FAILURE );
    } // if()
    else if (!pid) { // child process
      shmaddr = (int*)shmat(shmid, NULL, 0);
      copy(arr + pieces[a]->start, arr + pieces[a]->end + 1, shmaddr + pieces[a]->start);
      bubbleSort(shmaddr, pieces[a]->start, pieces[a]->end);
      shmdt(shmaddr);
      exit(EXIT_SUCCESS);
    } // else if()
    else pids.push_back(pid); // parent process
  } // for(a)

  for (int a = 0; a < pids.size(); a++) // wait all processes done
    waitpid(pids.at(a), nullptr, 0);


  while (pieces.size() > 1) { // Create threads and execute mergeSort
    int a = 0;
    if (pieces.size() % 2) pieces[pieces.size() - 1]->mid = 0; // to mark the lonely block

    while(a + 1 < pieces.size()) { // merge two blocks to one
      pieces[a]->mid = pieces[a + 1]->start;
      pieces[a]->end = pieces[a + 1]->end;
      pieces.erase(pieces.begin() + ++a);
    } // while()

    pids.clear();
    for (int b = 0; b < pieces.size(); b++) {
      pid_t pid = fork();

      if (pid == -1) { // error
        perror("fork error");
        exit( EXIT_FAILURE );
      } // if()
      else if (!pid && pieces[b]->mid) { // child process && We need to merge two blocks, don't merge the lonely block
        shmaddr = (int*)shmat(shmid, NULL, 0);
        mergeSort(shmaddr, pieces[b]->start, pieces[b]->mid, pieces[b]->end);
        shmdt(shmaddr);
        exit(EXIT_SUCCESS);
      } // else if()
      else pids.push_back(pid); // parent process
    } // for(b)

    for (int b = 0; b < pids.size(); b++) // wait all processes done
      waitpid(pids.at(b), nullptr, 0);
  } // while()

  shmaddr = (int*)shmat(shmid, NULL, 0); // Copy the data
  copy(shmaddr, shmaddr + size, arr); // from the virtual space back to arr
  shmdt(shmaddr);
  shmctl(shmid, IPC_RMID, NULL);

  pieces.clear();
  auto end = high_resolution_clock::now(); // Stop timing
  auto duration = duration_cast<milliseconds>(end - start); // Calculate execution time in milliseconds

  // for (int a = 0; a < size; a++) cout << arr[a] << "\n";
  printTime(duration);
} // method_3()


void method_4(int* arr, int size, int k) {
  auto start = high_resolution_clock::now(); // Start timing

  vector<pthread_t> threads;
  vector<piece*> pieces; // Split the array into pieces, where piece is a struct
  sliceToK(pieces, size, k); // containing the start and end positions of the block

  bubbleArgs** bubbleArgsArr = new bubbleArgs*[k];

  for (int a = 0; a < k; a++) { // Perform bubble sort on each data segment
    pthread_t t;
    bubbleArgsArr[a] = new bubbleArgs;
    bubbleArgsArr[a]->arr   = arr;
    bubbleArgsArr[a]->start = pieces[a]->start;
    bubbleArgsArr[a]->end   = pieces[a]->end;
    pthread_create(&t, NULL, bubbleSort, (void*)bubbleArgsArr[a]);  // create thread
    threads.push_back( t );
  } // for(a)

  for (int a = 0; a < threads.size(); a++) {
    pthread_join( threads.at(a), NULL );
    delete[] bubbleArgsArr[a];
  } // for(a)

  delete[] bubbleArgsArr;


  while (pieces.size() > 1) { // Create threads and execute mergeSort
    int a = 0;
    if (pieces.size() % 2) pieces[pieces.size() - 1]->mid = 0; // to mark the lonely block

    while(a + 1 < pieces.size()) { // merge two blocks to one
      pieces[a]->mid = pieces[a + 1]->start;
      pieces[a]->end = pieces[a + 1]->end;
      pieces.erase(pieces.begin() + ++a);
    } // while()

    threads.clear();
    mergeArgs** mergeArgsArr = new mergeArgs*[pieces.size()];

    for (int b = 0; b < pieces.size(); b++) {
      pthread_t t;

      if (pieces[b]->mid) { // merge two blocks, but don't merge the lonely block
        mergeArgsArr[b] = new mergeArgs;
        mergeArgsArr[b]->arr   = arr;
        mergeArgsArr[b]->start = pieces[b]->start;
        mergeArgsArr[b]->mid   = pieces[b]->mid;
        mergeArgsArr[b]->end   = pieces[b]->end;
        pthread_create(&t, NULL, mergeSort, (void*)mergeArgsArr[b]);
        threads.push_back(t);
      } // if()
    } // for(b)

    for (int a = 0; a < threads.size(); a++) {
      pthread_join(threads.at(a), NULL);
      delete[] mergeArgsArr[a];
    } // for(a)

    delete[] mergeArgsArr;
  } // while()

  pieces.clear();
  auto end = high_resolution_clock::now(); // Stop timing
  auto duration = duration_cast<milliseconds>(end - start); // Calculate execution time in milliseconds

  //for (int a = 0; a < size; a++) cout << arr[a] << "\n";
  printTime(duration);
} // method_4()


void printTime(milliseconds duration) {
  cout << "\nCPU Time : " << duration.count()<< "ms\n";

  stringstream result, offset; // Below is the code for handling Output Time
  time_t t = time(nullptr); // Get the current time
  tm local_tm = *localtime(&t);

  char buffer[80]; // Format the time string
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_tm);
  result << buffer;

  time_t utc = time(nullptr); // Get the local time zone
  tm utc_tm = *gmtime(&utc);
  int offset_hour = (difftime(mktime(&local_tm), mktime(&utc_tm)) / 3600);
  if (offset_hour >= 0) offset << "+";
  if (offset_hour < 10) offset << "0";
  offset << offset_hour << ":00";
  result << offset.str();

  cout << result.str() << "\n\n--------------------\n\n";
} // printTime()