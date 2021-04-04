// Michał Zobniów 307275
#include "sender.h"
#include <stdio.h>

u_int16_t computer_icmp_checksum(const void *buff, int length)
{
	u_int32_t sum;
	const u_int16_t *ptr = buff;
	assert(length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
	{
		sum += *ptr++;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

int send_icmp_packets(int sockfd, struct sockaddr_in recipient, int pid, int seq_num, int num_of_packets)
{

	// Setting up header
	struct icmp header;
	header.icmp_type = ICMP_ECHO;
	header.icmp_code = 0;
	header.icmp_id = pid;
	header.icmp_cksum = 0;

	// Sending (num_of_packets) packets with distinct icd_seq numbers
	for (int i = 0; i < num_of_packets; i++)
	{
		header.icmp_seq = seq_num;
		header.icmp_cksum = computer_icmp_checksum((u_int16_t *)&header, sizeof(header));
		if (sendto(sockfd, &header, sizeof(header), 0, (struct sockaddr *)&recipient, sizeof(recipient)) < 0)
		{
			return -1;
		}
	}
	return 1;
}
