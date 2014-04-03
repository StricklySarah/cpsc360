#include "simhttp.h"
  
// Function Prototypes
int requestReader(char *reqest, char *buff[], int length);
void makeResponse(char *response, char *version, int type, char* file);
void logRequest(char *meth, char *url, struct tm current, int response);
char *getDate(struct tm t);
char *getContentType(char *file);
  
int main(int argc, char **argv){
  extern int optind;
  extern char *optarg;
  uint16_t port = 8080;
  int c, listenfd, n, i;
  int respType, length;
  struct sockaddr_in  servaddr, from_address;
  struct tm *current;
  const time_t *now;
      
  int connfd;
  char buff[MAXLINE+1];
  char recvline[MAXLINE+1];
  char filePath[PATH_MAX];
  char *fileBuffer, *filePointer, *rootPointer, *root;
  char *request[50];
  char requestPath[100];
  time_t ticks;
  long fsize;
   
  while ((c = getopt(argc, argv, "p:")) != -1) {
    switch(c) {
      case 'p':
        port = atoi(optarg);
        break;
      case '?':
        fprintf(stderr,"unkown argument, %s \n", optarg);
        exit(1);
    }
    if((optind+1) > argc){
      fprintf(stderr,"not enough arguments, must enter directory \n");  
      exit(1);
    }
  }
 
    root = argv[optind];
  
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
    fprintf(stderr, "bind error\n");
    return 0;
  }
 
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family      = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port        = htons(port);
      
  if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0){
      fprintf(stderr, "bind error\n");
      return 0;
  }
      
  if ((listen(listenfd, LISTENQ)) < 0){
      fprintf(stderr, "listen error\n");
      return 0;
  }
              
  for ( ; ; ) {
    //accept blocks until an incoming connection arrives
    //it returns a "file descriptor" to the connection
    connfd = accept(listenfd, (SA *) NULL, NULL);
 
    // Get the current time
    time_t rawtime = time(NULL);
    struct tm timeinfo = *localtime(&rawtime);
    
    if (connfd < 0) {
        fprintf(stderr, "accept error");
    }
 
    //Now read the client's message.
    while ( (n = read(connfd, recvline, MAXLINE)) > 0) {
      recvline[n] = 0;    /* null terminate */
      //detect the end of the message.
      if (recvline[n-1] == '\n') {
        break;
      }
    }
    if (n < 0) {
      fprintf(stderr, "read err");
      return 0;
    }
 
    // Interpret the request
    length = requestReader(recvline, request, n);
 
    // Get the root path
    strcpy(requestPath, root);
    rootPointer = realpath(requestPath, rootPointer);
 
    strcat(requestPath, request[1]);
    // Get the file location
    if (request[1][strlen(request[1])-1] == '/') {
      strcat(requestPath, "index.html");
    }
    filePointer = realpath(requestPath, filePath);
 
    //now send a response.
    ticks = time(NULL);
 
    // Malformed packet
    if (packetMalformed(request, length)) {
      respType = 400;
    }
    // Request not found
    else if (filePointer == NULL) {
      respType = 404;
    }
    // Request outside of directory
    else if (strstr(filePointer, rootPointer) == NULL) {
      respType = 403;
    }
    // Request other than GET or HEAD
    else if ((strcmp(request[0], "HEAD") != 0) && (strcmp(request[0], "GET") != 0)) {
      respType = 405;
    }
    // Request was successful
    else {
      respType = 200;
    }
 
    makeResponse(buff, request[2], respType, filePointer);
    write(connfd, buff, strlen(buff));
    // If request is sucessful and method is GET
    if (respType == 200 && strcmp("GET", request[0]) == 0) {
      FILE *infile;
      infile = fopen(filePointer, "r");
      fseek(infile, 0, SEEK_END);
      fsize = ftell(infile);
      rewind(infile);
      fileBuffer = (char *)malloc(sizeof(char)*fsize);
      fread(fileBuffer, 1, fsize, infile);
      fclose(infile);   
      write(connfd, fileBuffer, fsize);
      free(fileBuffer);
    }
     
    // Print request information
    logRequest(request[0], request[1], timeinfo, respType);
    close(connfd);
  }
 
  free(filePointer);
  return 0;
}
  
// Function to handle the HTTP request
int requestReader(char *request, char *buff[], int length){  
  char *lines[MAXLINE+1];
  char *CR = strtok(request, "\r\n");
  char *sub = NULL;
  char *space;
  int i = 0;
  int j = 0;
  int k = 0;
  
  // Split request by line and store lines
  while(CR != NULL) {
    lines[i++] = CR;
    CR = strtok(NULL, "\r\n");
  }
 
  // Split lines by whitespace and store in buff
  for (k = 0; k < i; k++) {
    space = strtok(lines[k], " ");
    while(space != NULL) {
      buff[j++] = space;
      space = strtok(NULL, " ");
    }
  }
  return j;
}
 
void makeResponse(char *response, char *version, int type, char* file) {
    time_t curtime;
    struct stat sb;
  struct tm *timestamp, *modtime;
  char *typestring = (char *)malloc(20);
 
  time_t rawtime = time(NULL);
  struct tm timeinfo = *localtime(&rawtime);
 
  // Make appropriate response
  switch(type) {
    case 200:
      typestring = "OK";
      stat(file, &sb);
      struct tm filetime = *localtime(&(sb.st_mtime));
      sprintf(response, "%s %d %s\r\nDate: %s\r\nServer: S&SSuperServer\r\nLast -Modified: %s\r\nContent -Type: %s\r\nContent -Length: %d\r\nConnection: close\r\n\r\n",
      version, type, typestring, getDate(timeinfo),getDate(filetime), getContentType(file), (int)sb.st_size);
 
      break;
    case 400:
      typestring = "Bad Request";
      sprintf(response, "%s %d %s\r\nDate: %s\r\nServer: S&SSuperServer\r\nConnection: close\r\n\r\n",
      version, type, typestring, getDate(timeinfo));
      break;
    case 403:
      typestring = "Forbidden";
      sprintf(response, "%s %d %s\r\nDate: %s\r\nServer: S&SSuperServer\r\nConnection: close\r\n\r\n",
      version, type, typestring, getDate(timeinfo));
      break;
    case 404:
      typestring = "Not Found";
      sprintf(response, "%s %d %s\r\nDate: %s\r\nServer: S&SSuperServer\r\nConnection: close\r\n\r\n",
      version, type, typestring, getDate(timeinfo));
      break;
    case 405:
      typestring = "Method Not Allowed";
      sprintf(response, "%s %d %s\r\nDate: %s\r\nServer: S&SSuperServer\r\nAllow: GET, HEAD\r\nConnection: close\r\n\r\n",
      version, type, typestring, getDate(timeinfo));
      break;
  }
}
 
// Function to print the request information
void logRequest(char *meth, char *url, struct tm t, int response) {
  fprintf(stdout, "%s\t%s\t%s\t%d\n", meth, url, getDate(t), response);
}
 
// Function to return the date as a char *
char *getDate(struct tm t) {
  char *dateString = (char *) malloc(sizeof(char) * 20);
  char* month[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  char* day[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
 
  sprintf(dateString, "%s, %i %s %d %.2d:%.2d", day[t.tm_wday], t.tm_mday,
                  month[t.tm_mon], t.tm_year+1900, t.tm_hour, t.tm_min);
  return dateString;
}
 
// Function to check for a malformed request
int packetMalformed(char *request[], int length) {
  int i;
  // Make sure host header is included
  for (i = 0; i < length; i++) {
    if (strcmp(request[i], "Host:") == 0) {
      return 0;
    }
  }
  return 1;
}
 
// Function to return the correct content type
char *getContentType(char *filepath) {
  int i;
  char *type = (char *) malloc(sizeof(char) * 30);
  char *extension;
  extension = strrchr(filepath, '.');
  char *exts[7] = {".css", ".html", ".htm", ".js", ".txt", ".jpg", ".pdf"};
  char *types[7] = {"text/css", "text/html", "text/htm", "application/javascript",
                    "text/plain", "image/jpg", "application/pdf"};
  for (i = 0; i < 7; i++) {
    if(strcmp(extension, exts[i]) == 0) {
      return types[i];
    }
  }
  return "applications/octet-stream";
}
