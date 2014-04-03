int main(int argc, char **argv){
  extern int optind;
  extern char *optarg;

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
}
