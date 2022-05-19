/* ... */
  struct addrinfo *ptr = res;
  while (ptr) {
    printf("Result: ");
    if (ptr->ai_addr->sa_family == AF_INET) {
      char ip4[INET_ADDRSTRLEN];
      struct sockaddr_in *sin_val = (struct sockaddr_in *) ptr->ai_addr;
      inet_ntop(AF_INET, &(sin_val->sin_addr), ip4, INET_ADDRSTRLEN);
      printf("Socket on address:port %s:%d\n", ip4, ntohs(sin_val->sin_port));
    } else if (ptr->ai_addr->sa_family == AF_INET6) {
      char ip6[INET6_ADDRSTRLEN];
      struct sockaddr_in6 *sin6_val = (struct sockaddr_in6 *) ptr->ai_addr;
      inet_ntop(AF_INET6, &(sin6_val->sin6_addr), ip6, INET6_ADDRSTRLEN);
      printf("Socket on [%s]:%d\n", ip6, ntohs(sin6_val->sin6_port));
    } else {
      printf("Not an IPv4 nor an IPv6\n");
    }
    ptr = ptr->ai_next;
  }
  return 0;
}
