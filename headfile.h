/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   headfile.h                                                               */
/*                                                                            */
/*   By: JianMing <jason.kang1106@gmail.com>                                  */
/*                                                                            */
/*   Created: 2017/10/24 20:37:27 by JianMing                                 */
/*   Updated: 2017/10/24 20:37:38 by JianMing                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACCOUNT_RECEIVABLE_H
#define ACCOUNT_RECEIVABLE_H
#include <fstream>

struct master
{
	int		cust_num;
	std::string	first_name;
	std::string	last_name;
	double		balance;
};

struct transactions
{
	char		type;
	int		cust_num;
	int		trans_num;
	std::string	item;
	int		quantity;
	double		price;
	double		amtpaid;
};

#endif

