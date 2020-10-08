#include <iostream>
#include <vector>
#include <fstream>
#include <omp.h>

using namespace std;
int main()
{
    omp_set_num_threads(8);
    clock_t start, end;
    start = clock();
    int tamlinha, tamcoluna;
    char discart;
    cin >> tamlinha;
    cin >> discart;
    cin >> tamcoluna;
    // cin>>discart;
    vector<vector<double> > matriz(tamlinha, vector<double>(tamcoluna));

    for (int i = 0; i < tamlinha; i++)
    {
        for (int j = 0; j < tamcoluna; j++)
        {
            cin >> matriz[i][j];
            if (j != tamcoluna - 1)
                cin >> discart;
        }
    }

    int coluna = 0;
    for (int linha = 0; linha < matriz.size(); linha++)
    {
        //if the number in the start column is 0, try to switch with another
        while (matriz[linha][coluna] == 0.0)
        {
            bool flag = false;
            int i = linha;
            while (!flag && i < matriz.size())
            {
                if (matriz[i][coluna] != 0)
                {

                    double temp[tamlinha];
#pragma omp parallel 
                    {

#pragma omp for

                        for (int h = 0; h < tamlinha; h++)
                        {
                            temp[h] = matriz[linha][h];
                        }
#pragma omp for

                        for (int h = 0; h < tamlinha; h++)
                        {
                            matriz[linha][h] = matriz[i][h];
                        }
#pragma omp for

                        for (int h = 0; h < tamlinha; h++)
                        {
                            matriz[linha][h] = temp[h];
                        }
                    }
                    //double temp[tamlinha] = matriz[i];
                    //matriz[i] = matriz[linha];
                    //matriz[linha] = temp;
                    flag = true;
                }
                i++;
            }
            //if after trying to switch, it is still 0, increase column
            if (matriz[linha][coluna] == 0.0)
            {
                coluna++;
            }
        }
        //System.out.println("run op1");

        //System.out.println(toString());

        //if the number isn't one, reduce to one
        if (matriz[linha][coluna] != 1.0)
        {
            double divisor = matriz[linha][coluna];
            #pragma omp parallel
            {
            #pragma omp for    
            for (int i = coluna; i < matriz[linha].size(); i++)
            {
                matriz[linha][i] = matriz[linha][i] / divisor;
            }
            }
        }

        ///=========================

        //System.out.println("run op2");

        //System.out.println(toString());

        //make sure the number in the start column of all other linhas is 0
        for (int i = 0; i < matriz.size(); i++)
        {
            if (i != linha && matriz[i][coluna] != 0)
            {
                double multiple = 0 - matriz[i][coluna];
                #pragma parallel
                {
                    #pragma omp for
                for (int j = coluna; j < matriz[linha].size(); j++)
                {
                    matriz[i][j] +=
                        multiple * matriz[linha][j];
                }
                }
            }
        }
        //System.out.println("run op3");

        //System.out.println(toString());

        coluna++;
    }

    end = clock();

    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
/*
    for (int i = 0; i < tamlinha; i++)
    {
        for (int j = 0; j < tamcoluna; j++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
*/
    //cout << fixed << setprecision(5);
    cout << "\nTempo de execucao:\n"
         << time_taken << " segundos\n";
          ofstream writer;
    writer.open("output.txt");

/*
    ofstream writer;
    writer.open("output.txt");

    for (int i = 0; i < tamlinha; i++)
    {
        for (int j = 0; j < tamcoluna; j++)
        {
            writer << matriz[i][j] << " ";
        }
        writer << endl;
    }
*/
    writer << "\nTempo de execucao:\n"
           << time_taken << " segundos\n";
    writer.close();

    return 0;
}
