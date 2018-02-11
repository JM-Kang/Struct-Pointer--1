/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   assignment1.cc                                                           */
/*                                                                            */
/*   By: JianMing <jason.kang1106@gmail.com>                                  */
/*                                                                            */
/*   Created: 2017/10/24 20:25:30 by JianMing                                 */
/*   Updated: 2017/10/25 10:55:57 by JianMing                                 */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "headfile.h"
using namespace std;

void	testfile(void);
void	test_CXN_duplicate(int, int&);
void	test_TXN_typeO(int, int&, int&);
void	cal_order(int&, int);
void	display_Bal(int);
void	test_TXN_typeP(int, int&, int&);
void	cal_payment(int&, int);

master		mast[10];
transactions 	trans[26];
ifstream	inmaster;
ifstream	intrans;
ofstream	outfile("file.txt");

int	main()
{
	int n, j, CN_out, TN_out, dupl_N, temp;	

	outfile.setf(ios::fixed, ios::floatfield);
	outfile.precision(2);

	//testing the accessible for both files
	inmaster.open("master.txt");
	if(!inmaster)
		cout << "Cannot open Master Record file, please check" << endl;

	intrans.open("transaction.txt");
	if(!intrans)
		cout << "Cannot open Transactions file, please check" << endl;

	j = 0;
	n = 0;
	CN_out = 0;
	TN_out = 0;
	dupl_N = 0;
	temp = 0;

	//using while loop input master record file data.
	inmaster >> mast[j].cust_num;
	while(inmaster)
	{
		//test customer number for duplucation
		//print out customer previous information
		test_CXN_duplicate(j, CN_out);
		if(CN_out == -1)
			break;
		display_Bal(j);

		//using while loop to intup transactions file data
		if(trans[n].type == '\0')
			intrans >> trans[n].type;
		trans[0].cust_num = 0;
		while(trans[n].type == 'O' || trans[n].type == 'P' || trans[n].type == '\0')
		{	
			if(!trans[n].cust_num)
				intrans >> trans[n].cust_num;
			
			//if the transaction is order
			//both file customer number are same then input the data
			if(mast[j].cust_num == trans[n].cust_num && trans[n].type == 'O')
			{
				intrans >> trans[n].trans_num;
				
				//test the transaction number for duplication
				test_TXN_typeO(n, TN_out, dupl_N);
				if(TN_out == -1)
					break;
				
				//if non-duplicated number then caculate the order amount
				if(dupl_N <= temp)
					cal_order(n,j);

				//if duplicated number then input the next data
				else
				{
					temp = dupl_N;
					intrans >> trans[n].type;
					intrans >> trans[n].cust_num;
				}
			}
			
			//if the transaction is payment
			//both file customer number are same then input the data
			else if(mast[j].cust_num == trans[n].cust_num && trans[n].type == 'P')
			{
				intrans >> trans[n].trans_num;
				
				//test the transaction number for duplication
				test_TXN_typeP(n, TN_out, dupl_N);
				if(TN_out == -1)
					break;
				
				//if non-duplicated number, then calculate payment
				if(dupl_N <= temp && dupl_N != -1)
					cal_payment(n, j);
				
				//if duplicated number, then input next data
				else
				{
					temp = dupl_N;
					intrans >> trans[n].type;
					intrans >> trans[n].cust_num;
				}
			}
				
			//if both file customer number are different then output the balance	
			else
			{
				outfile << endl << "\t" << "BALANCE DUE:";
				outfile << "\t" << "$" << mast[j].balance;
				outfile << endl << endl << endl << endl << endl;
				break;
			}
		}
		j++;
		inmaster >> mast[j].cust_num;
	}
	return 0;
}

//display customers information and previous balance
void	display_Bal(int j)
{
	inmaster >> mast[j].first_name;
	inmaster >> mast[j].last_name;
	inmaster >> mast[j].balance;

	outfile << mast[j].first_name << " " << mast[j].last_name;
	outfile << "\t" << "CN#" << mast[j].cust_num << endl;
	outfile << "\t" << " PREVIOUS BALANCE:" << "\t";
	outfile << "$" << mast[j].balance << endl << endl;
}

//testing the customer number for duplication
void	test_CXN_duplicate(int j, int& C_out)
{
	for(int i = 0; i < j; i++)
	{	
		//if number is same, input next set data
		if(mast[j].cust_num == mast[i].cust_num)
		{
			inmaster >> mast[j].first_name;
			inmaster >> mast[j].last_name;
			inmaster >> mast[j].balance;

			cout << "Cutomer number duplicate: ";
			cout << mast[j].cust_num << endl;
			
			//if customer data out, C_out becomes negative one
			if(j >= (9 - C_out))
			{
				C_out = -1;
				break;
			}
			inmaster >> mast[j].cust_num;
			C_out++;
			i = 0;
		}
	}
}

//testing the transaction number with type O for duplication
void	test_TXN_typeO(int n, int& T_out, int& dupl_N)
{
	for(int m=0; m<n; m++)
	{
		//if transaction number is same, input the rest set of data
		if(trans[m].trans_num == trans[n].trans_num)
		{
			intrans >> trans[n].item;
			intrans >> trans[n].quantity;
			intrans >> trans[n].price;
			cout << "Transaction number duplicate: ";
			cout << trans[n].trans_num << endl;
			dupl_N++;

			//if transcation file data out, T_out becomes negative one
			if(n >= (25 - dupl_N))
				T_out = -1;
			break;
		}
	}
}

//calculate the order amount
void	cal_order(int& n, int j)
{
	intrans >> trans[n].item;
	intrans >> trans[n].quantity;
	intrans >> trans[n].price;

	//caculate the balance
	mast[j].balance += trans[n].price * trans[n].quantity;
	outfile <<"TN#" << trans[n].trans_num << "\t\t";
	outfile << trans[n].item << "\t\t" << "$";
	outfile << trans[n].price * trans[n].quantity << endl;
	
	n++;
	intrans >> trans[n].type;
}

//testing the transaction number with type P for duplication
void	test_TXN_typeP(int n, int& T_out, int& dupl_N)
{
	//if transaction number is same, input the rest set of data
	for(int k=0; k<n; k++)
	{
		if(trans[k].trans_num == trans[n].trans_num)
		{
			intrans >> trans[n].amtpaid;
			cout << "Transaction number duplicate: ";
			cout << trans[n].trans_num <<endl;
			dupl_N++;
			
			//if transaction file out data, T_out becomes negative one
			if(n >= (25 - dupl_N))
				T_out = -1;
			break;
		}
	}
}

//calculate the payment
void	cal_payment(int& n, int j)
{
	intrans >> trans[n].amtpaid;
	mast[j].balance -=trans[n].amtpaid;
	
	//display information
	outfile << "TN#" << trans[n].trans_num << "\t\t";
	outfile << "PAYMNET" << "\t\t\t" <<"$";
	outfile << trans[n].amtpaid << endl;
	n++;
	intrans >> trans[n].type;
}
