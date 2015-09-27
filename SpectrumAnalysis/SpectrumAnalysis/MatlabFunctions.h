

double fix(double Number); // function for get num without .xxx part

int nextpow2(int b);  // poluchenie pokazatelya stepeni

double* feval(int NFFT, char* window, int Lfrm);  // function for evaluation window parameters

double sum(double* mas, int size); // summirovanie elementov massiva

double mean(double* vec, int len);  // srednee znachenie v vectore

double std_vec(double* vec, int len); // standartnoe otklonenie vectora

double* pow_vec(double* vec, int len, double n);  // ne rabotaet pri pereprisvaivanii

double* abs_vec(double* vec, int len); // absolutnoe znachenie vectora

double norm(double* v, int len, double p); // norma vectora