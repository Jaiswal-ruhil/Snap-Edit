/* Contains all the functions required by sit.c */


void readfileintobuffer(FILE *fptr)  
{
  int tracklinenum=1;
  char *tempstr; //stores strings from the file line-by-line
  int tempstrcur=0;
  int tempstrmax=49;
  unsigned char c;
  HANDLE_CALLOC(tempstr,50,sizeof(char));
        
  while(!(feof(fptr))) //checking for end of file
    {
      c=fgetc(fptr); // retreives a character and advances pointer
      if(c=='\n')  //when a whole line has been copied into the string
        {       replace(tracklinenum,tempstr); 
          memset(tempstr,'\0',tempstrmax);
          tracklinenum++;
          tempstrcur=0;
        }
      else if(tempstrcur<tempstrmax)
        {
          tempstr[tempstrcur] = c;
          tempstrcur++; 
        }       
      else  //tempstr doesn't have enough space to hold the characters from the line
        {       
          HANDLE_REALLOC(tempstr,tempstrmax+50);
          tempstrmax+=50;
          tempstr[tempstrcur]=c;
          tempstrcur++;
                        
        }
                

    }
        
} 

line * findline(int l) //returns a pointer to the (structure of) line number passed
{
  line *temp;
  if(l==1)
    return buffer;
  if(l > lastline->thisline)
    return NULL;
  for(temp=buffer;(temp->thisline) < l;temp=temp->next);
  return temp;
}


void buildbuffertill(int l) 
{
  if (l<=(lastline->thisline))
    return;     
  else
    {   int temp=lastline->thisline;
      do
        {
          HANDLE_CALLOC(lastline->next,sizeof(line),1);
          lastline=lastline->next;
          HANDLE_CALLOC(lastline->linestring,INITIAL_CHAR_MAX,sizeof(char));
          lastline->thisline=temp+1;
          temp++;
                        
        }       
      while(lastline->thisline < l);
                
      return;
                

    }
}


void allocstring(line *p) //(consider #defining function code)
{
        
  HANDLE_CALLOC(p->linestring,sizeof(char),INITIAL_CHAR_MAX);
  p->maxsize=INITIAL_CHAR_MAX;
  return;
        
}

void expandstring(line *p)  //consider #defining function code
{
  HANDLE_REALLOC(p->linestring,sizeof(char)*((p->maxsize)+50));
  (p->maxsize) += 50;
  return;
}

int delete(int l) 
{
  int templinenum=l;
  line *temp;
  savebeforequit=1;     
  if(l==1)
    {   
      if(lastline->thisline==1)
        {                       
          memset(buffer->linestring,'\0',buffer->maxsize);
          return 0;
        }
      line *p=buffer->next;
      for(temp=p;temp->next;temp=temp->next,templinenum++)//Changing the line numbers of the lines after the deleted line
        temp->thisline=templinenum;
      free(buffer->linestring);
      free(buffer);     
      buffer=p;
      return 0;
    }
  else if(l< (lastline->thisline) && l>0)
    {   
      line *p=findline(l-1);
      line *p1= p->next;
      line *p2 = p1->next;
      temp=p2;
      for(;temp->next;temp=temp->next,templinenum++)
        temp->thisline=templinenum;
      free(p1->linestring);
      free(p1);
      (p->next)=p2; //connecting the structures
                
      return 0;
    }
  else if(l==(lastline->thisline))
    {
      line *p=findline(l-1);    
                
      free(lastline->linestring);
      free(lastline);
      p->next = NULL;
      lastline=p;

    }

  else return LINE_ERROR;
        
}


int replace(int l, char *string)
{       
  int i;        
  int stringlength=strlen(string);
  line *p=findline(l);
  savebeforequit=1;
  if(!p)// findline() returns NULL if line hasn't been built yet
    {   buildbuffertill(l);
      p=findline(l);
    }   
  if(!(p->linestring)) // check if string on he line has been allocated memory
    allocstring(p);
        
  while((p->maxsize) < (stringlength + 1))
    {
      expandstring(p);
    }   
  strcpy((*p).linestring,string); 
  return 0;
}

int append(int l, char *string)
{

  line *p=findline(l);
  savebeforequit=1;
  if(!p)// findline() returns NULL if line hasn't been built yet
    {   buildbuffertill(l);
      p=findline(l);    
      replace(l,string);
      return 0;
    }
  else
    {   

      int linelength;
      int stringlength;
                
      if(!(p->linestring))
        allocstring(p); 
      linelength=strlen(p->linestring);
      stringlength=strlen(string);      
                
      while((p->maxsize )< (linelength + stringlength + 1) )
        expandstring(p);
      strcpy((p->linestring)+(linelength),string);
      return 0;
    }
} 

int print()
{       line *temp;
  int line=1;
  putchar('\n');
  printf("-------------------------------------------------\n\t\t%s\n-------------------------------------------------\n",filename);
  for( temp=buffer ;temp!=lastline; temp=(temp->next),line++) // breaks at the last line
    {   printf("% -6d |",line); 
      if(!(temp->linestring))//ignore lines without initialized strings in them
        {putchar('\n'); continue;}
      printf("%s \n",temp->linestring);
    }
  printf("% -6d |",line);
  if(temp->linestring)
    printf("%s \n",temp->linestring);//prints the last line     
  return 0;     
}

int write(FILE **fptr)
{
  char remove[40]="rm ";
  savebeforequit=0;
  line *temp=buffer;
  strcat(remove,filename);
  system(remove); // deleting filestream and get rid of all the previous text
  *fptr=fopen(filename,"w+"); // opening new filestream with same name
  //figure out better way to flush a file
  for( ;temp!=lastline; temp=(temp->next)) // breaks at the last line
    {   if(!(temp->linestring))//ignore lines without initialized strings in them
        {fprintf(*fptr,"\n");
          continue;}
      fprintf(*fptr,"%s\n",temp->linestring);
    }
  if(temp->linestring)
    fprintf(*fptr,"%s\n",temp->linestring);//prints the last line
  return 0;

}


void displayhelp()
{
  printf("%-25s %s","\n\n\'replace l<foo> |bar\'","  to replace the content at line \'foo\' with string \'bar\'\n");
  printf("%-25s %s","\'append l<foo> |bar\'","to add \'bar\' at the end of line \'foo\'\n");
  printf("%-25s %s","\'delete l<foo>\'","to delete line \'foo\'\n");
  printf("%-25s %s","\'write\'","to save the file in the buffer into the file\n");
  printf("%-25s %s","\'print\'","to view the buffer\n");        
  printf("%-25s %s","\'help\'","to display this again\n");      
  printf("%-25s %s","\'quit\'","to end program\n\n"); 
  return;
        
}






void processraw(char *rawin,char *string)
{
  int i,j;
  char *tempcommand=(char *)calloc(sizeof(char),50);
  char *linenumstring=(char *)calloc(sizeof(char),50);
  int templine=0;
  //copying command     
                        
  for(i=0;rawin[i]!=' '&&rawin[i]!='\0';i++)
    tempcommand[i]=rawin[i];
  //null character to end string
  tempcommand[i]='\0';
  //comparing strings to determine command
        
  if(!strcmp(tempcommand,"delete"))
    command=DELETE;
  else if(!strcmp(tempcommand,"append"))
    command=APPEND;
  else if(!strcmp(tempcommand,"replace"))
    command= REPLACE;   
  else if(!strcmp(tempcommand,"quit"))
    command= QUIT;
  else if(!strcmp(tempcommand,"print"))
    command= PRINT;
  else if(!strcmp(tempcommand,"write"))
    command= WRITE;
  else if(!strcmp(tempcommand,"help"))
    command= HELP;      
  else
    {   
      command= COMMAND_ERROR;
      return;
    }

  //Determining line number
  if(command==DELETE||command==APPEND||command==REPLACE)
    {
      i++;
      if(rawin[i]=='l')
        {       
                        
          i++;                  
          for(j=0;rawin[i]!=' '&&rawin[i]!='\0';i++,j++)
            linenumstring[j]=rawin[i];
          linenumstring[++j]='\0';
          templine=atoi(linenumstring);
                                        
          if(templine>0)
            linenum=templine;
          else
            {
              command=LINE_ERROR;
              return;                   
            }           
        }       
      else
        {command=LINE_ERROR; return;}
    }
  //copying input string prefixed with '|'
  if(command==APPEND||command==REPLACE)
    {
      i++;
      if(rawin[i]=='|')
        {
          i++;
          strcpy(string,&rawin[i]);     
        }
      else
        {command = COMMAND_ERROR; return;}
    }
}
