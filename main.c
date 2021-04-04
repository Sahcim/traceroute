#include <stdio.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include "sender.h"
#include "reciver.h"

double avg_time(struct timeval timestamps[], struct timeval start_time, int responses)
{
	double avg = 0;
	for (int i = 0; i < responses; i++)
	{
		long seconds = timestamps[i].tv_sec - start_time.tv_sec;
		long microseconds = timestamps[i].tv_usec - start_time.tv_usec;
		avg += (seconds + microseconds * 1e-6) * 1000;
	}
	return avg / responses;
}

void print_ips(int responses, char ip[][16])
{
	printf("IP address:");
	char ip_to_print[16];
	bool is_same;

	for (int i = 0; i < responses; i++)
	{
		strcpy(ip_to_print, ip[i]);
		is_same = true;

		for (int j; j < i; j++)
		{
			if (strcmp(ip_to_print, ip[i]) == 0)
			{
				is_same = false;
				break;
			}
		}
		if (is_same)
			printf(" %s", ip_to_print);
	}
}

void print_stats(int responses, struct timeval start_time, struct timeval timestamps[], char ip[][16])
{
	if (responses == 0)
	{
		printf("???\n");
		return;
	}

	printf("Avg time: %.0f ms ", avg_time(timestamps, start_time, responses));
	print_ips(responses, ip);
	printf("\n");
}

int main(int argc, char *argv[])
{

	if (argc < 2)
	{
		printf("No address was given/too many arguments, try again");
		return EXIT_FAILURE;
	}

	// constants
	const int NUM_OF_PACKETS = 3;
	const int MAX_TTL = 30;
	const int TIMEOUT_IN_SECONDS = 1;

	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP); // Setting up socket
	if (sockfd < 0)
	{
		fprintf(stderr, "Error while creating socket: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	// Setting up recipient parameters (ip etc)
	struct sockaddr_in recipient;
	bzero(&recipient, sizeof(recipient));
	recipient.sin_family = AF_INET;
	if (inet_pton(AF_INET, argv[1], &recipient.sin_addr) < 0)
	{
		fprintf(stderr, "Wrong IPv4 address, try again: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}

	// ttl variable
	int ttl;

	// id and seq variable
	int pid = (int)getpid();
	int seq_random;

	// variables for printing stats
	int responses;							   // number_of_recived packets
	struct timeval start_time;				   // start time
	struct timeval timestamps[NUM_OF_PACKETS]; // times of packets reciving
	char ip[NUM_OF_PACKETS][16];			   // ip adresses of recived packets

	// recive status to check for echo_reply
	int recive_status;

	for (int i = 1; i <= MAX_TTL; i++)
	{
		//seting ttl to i
		ttl = i;
		setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
		// reseting seq_number, num_of_responses, start_time
		seq_random = rand() % 10000;
		responses = 0;
		gettimeofday(&start_time, 0);

		//sending packets
		if (send_icmp_packets(sockfd, recipient, pid, seq_random, NUM_OF_PACKETS) == -1)
		{
			fprintf(stderr, "Error while sending packets %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
		//reciving packets
		recive_status = recive_icmp_packets(sockfd, pid, seq_random, NUM_OF_PACKETS, TIMEOUT_IN_SECONDS,
											&responses, ip, timestamps);
		if (recive_status < 0)
		{
			fprintf(stderr, "Error while reciving packets %s\n", strerror(errno));
			return EXIT_FAILURE;
		}
		//printing stats
		printf("TTL: %d ", ttl);
		print_stats(responses, start_time, timestamps, ip);

		if (recive_status == 2)
		{
			break;
		}
	}

	return EXIT_SUCCESS;
}
