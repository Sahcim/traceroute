// Michał Zobniów 307275
#include "reciver.h"

int recive_icmp_packets(int sockfd, int pid, int seq_num, int num_of_packets, int wait_time_in_seconds,
						int *responses, char ip[][16], struct timeval timestamps[])
{

	struct timeval begin, end;
	gettimeofday(&begin, 0);
	long seconds;
	long microseconds;
	double elapsed = 0;

	// Select setup
	int select_status;
	struct timeval tv;
	tv.tv_sec = wait_time_in_seconds;
	tv.tv_usec = 0;
	fd_set descriptors;
	FD_ZERO(&descriptors);
	FD_SET(sockfd, &descriptors);
	// recive setup
	struct sockaddr_in sender;
	socklen_t sender_len = sizeof(sender);
	u_int8_t buffer[IP_MAXPACKET];
	// Packet unpacking variables
	struct ip *ip_header;
	struct icmp *icmp_header;
	// Destination reached
	bool is_echo;

	while ((*responses) < num_of_packets && elapsed <= wait_time_in_seconds)
	{
		select_status = select(sockfd + 1, &descriptors, NULL, NULL, &tv);

		if (select_status < 0)
		{
			return -1;
		}
		else if (select_status == 0)
		{
			break;
		}
		else
		{
			for (;;)
			{
				if (recvfrom(sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT,
							 (struct sockaddr *)&sender, &sender_len) < 0)
				{
					if (errno == EWOULDBLOCK)
					{
						break;
					}
					return -1;
				}
				if ((*responses) >= num_of_packets)
					break;

				ip_header = (struct ip *)buffer;
				icmp_header = (struct icmp *)(buffer + 4 * ip_header->ip_hl);

				if (icmp_header->icmp_type == ICMP_TIME_EXCEEDED || icmp_header->icmp_type == ICMP_ECHOREPLY)
				{
					if (icmp_header->icmp_type == ICMP_TIME_EXCEEDED)
					{
						ip_header = (struct ip *)((uint8_t *)icmp_header + sizeof(struct icmphdr));
						icmp_header = (struct icmp *)((uint8_t *)ip_header + ip_header->ip_hl * 4);
					}

					if (seq_num == icmp_header->icmp_seq &&
						icmp_header->icmp_id == pid)
					{
						if (icmp_header->icmp_type == ICMP_ECHOREPLY)
							is_echo = true;

						inet_ntop(AF_INET, &sender.sin_addr, ip[(*responses)], sizeof(char[16]));
						gettimeofday(&timestamps[(*responses)], 0);
						(*responses)++;
					}
				}
			}
			// time update
			gettimeofday(&end, 0);
			seconds = end.tv_sec - begin.tv_sec;
			microseconds = end.tv_usec - begin.tv_usec;
			elapsed = seconds + microseconds * 1e-6;
			tv.tv_sec = wait_time_in_seconds - seconds;
			tv.tv_usec = 1e6 - microseconds;
		}
	}

	return is_echo ? 2 : 1;
}
