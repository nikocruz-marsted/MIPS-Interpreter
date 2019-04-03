#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "mips.h"

/************** BEGIN HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ***************/
const int R_TYPE = 0;
const int I_TYPE = 1;
const int MEM_TYPE = 2;
const int UNKNOWN_TYPE = 3;

/**
 * Return the type of instruction for the given operation
 * Available options are R_TYPE, I_TYPE, MEM_TYPE, UNKNOWN_TYPE
 */
int get_op_type(char *op) {
    const char *r_type_op[] = { "addu", "subu", "and", "or", "xor", "nor", "slt", "sltu" , "movn", "movz", "sllv", "srlv", "srav" };
    const char *i_type_op[] = { "addiu", "andi", "ori", "xori", "slti", "sltiu", "sll", "srl", "sra", "lui" };
    const char *mem_type_op[] = { "lw", "lb", "lbu", "sw", "sb" };
    for (int i = 0; i < (int) (sizeof(r_type_op) / sizeof(char *)); i++) {
        if (strcmp(r_type_op[i], op) == 0) {
            return R_TYPE;
        }
    }
    for (int i = 0; i < (int) (sizeof(i_type_op) / sizeof(char *)); i++) {
        if (strcmp(i_type_op[i], op) == 0) {
            return I_TYPE;
        }
    }
    for (int i = 0; i < (int) (sizeof(mem_type_op) / sizeof(char *)); i++) {
        if (strcmp(mem_type_op[i], op) == 0) {
            return MEM_TYPE;
        }
    }
    return UNKNOWN_TYPE;
}
/*************** END HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ****************/

registers_t *registers;
// TODO: create any additional variables to store the state of the interpreter
hashtable_t *table;
void init(registers_t *starting_registers) {
    registers = starting_registers;
    // TODO: initialize any additional variables needed for state
    table = ht_init(300);
    //initialize memory using hashtable

}

// TODO: create any necessary helper functions

//write hex to decimal converter and vice versa

char *array[4];// array to store opcode and arguments. starts at 0. 
//IF the argument only has 3 elements(e.g lw $t1, 3($t5)) plz don't try to access fourth element in array 

void Parse_Args(char* instruction){
//stores arguments into array[] above
// resolve for sb  $10, 0x11 ( $0 ), in this case $0 will be in the next register after 11
// array[1] = $10
// array[2] = 0x11
//array[3] = $0
int i = 0;

char *p = strtok(instruction, " ,()");

while (p!=NULL){
   
        array[i++] = p;
       
      
        
    
    p = strtok(NULL, " ,()");
}
}

int power(int x, int y) 
{ 
    if (y == 0) 
        return 1; 
    else if (y%2 == 0) 
        return power(x, y/2)*power(x, y/2); 
    else
        return x*power(x, y/2)*power(x, y/2); 
} 

// used to extract the hex value, removes 0x, so 0x111 becomes int 273 in decimal
//IF ARGUMENT IS NOT HEX THEN THIS FUNCTION RETURNS 0
// add SIGN EXTENSION?
// if signExtend = 1, then sign extend, else 0 extend
// also need to convert to two complement?
int Get_Hex(char* pointer){
    if  ((strpbrk(pointer, "x")) == NULL){
    //not hex 
        return 0;
    }
    
char* result = strtok(pointer, "x");
result = strtok(NULL, "x");
int len = strlen(result); 
int base = 1;
int sum = 0;
for (int i=len-1; i>=0; i--) {
    if (result[i] >= '0' && result[i] <= '9'){
        sum += (result[i] - 48)*base; 
        base = base*16;
    }
    else if (result[i] >= 'a' && result[i] <= 'f'){
        sum += (result[i]- 87)*base;
        base = base*16;
    }
}
return sum;
}

int signExtension(int instr) {
    int value = (0x0000FFFF & instr);
    int mask = 0x00008000;
    if (mask & instr) {
        value = value | 0xFFFF0000;
    }
    return value;
}
int zeroExtension(int instr) {
    int value = (0x0000FFFF & instr);// is this the right way
    return value;
}



//int convert()
void step(char *instruction) {
    // Extracts and returns the substring before the first space character,
    // by replacing the space character with a null-terminator.
    // `instruction` now points to the next character after the space
    // See `man strsep` for how this library function works
    // need to add checks for instructions that have the possibility of changing register $zero
    // load into memory as little endian 
    Parse_Args(instruction);
    
    char *op = strsep(&instruction, " ");
    // Uses the provided helper function to determine the type of instruction
    int op_type = get_op_type(op);
    // Skip this instruction if it is not in our supported set of instructions
    if (op_type == UNKNOWN_TYPE) {
        return;
    }
    if(atoi(strtok(array[1], "$")) == 0){
      return;
    }

    int Dec_int = 0;
    if  ((strpbrk(array[3], "x")) != NULL){
      Dec_int = Get_Hex(array[3]);
    }
    int array_2 = 0;
    if  ((strpbrk(array[2], "x")) != NULL){
      array_2 = Get_Hex(array[2]);
    }
    
    if (op_type == 0){
        if (strcmp(op, "addu") == 0){
            registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))] + registers->r[atoi(strtok(array[3], "$"))];
            // need atoi to change from char to int, strtok to get rid of $ sign
        }

        if (strcmp(op, "subu") == 0){
            registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))] - registers->r[atoi(strtok(array[3], "$"))];
        }

        if (strcmp(op, "and") == 0){
            registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))]&registers->r[atoi(strtok(array[3], "$"))];

        }
         if (strcmp(op, "or") == 0){
            registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))]|registers->r[atoi(strtok(array[3], "$"))];
        }  
         if (strcmp(op, "xor") == 0){
            registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))]^registers->r[atoi(strtok(array[3], "$"))];
        } 
       
        if (strcmp(op, "slt") == 0){
            if (registers->r[atoi(strtok(array[2], "$"))] < registers->r[atoi(strtok(array[3], "$"))]){
                registers->r[atoi(strtok(array[1], "$"))] = 1;
            } else{
                registers->r[atoi(strtok(array[1], "$"))] = 0;
	    }
        }
	if (strcmp(op, "movz") == 0){	  
	  if (registers->r[atoi(strtok(array[3], "$"))] == 0){
	    registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))];
	  }
	}
	

    } else if (op_type == 1){
        //do i need to convert hex to decimal first? what about sign extending? in the conversion process do i have to turn it into a two's complement frist
        // are the values in register 32-bye hex or decimals?

        //ARE WE ALLOWED TO ADD DECIMAL TO HEX
        if (strcmp(op, "addiu") == 0){
           //add upper immediate rt to rs and then load it to rd, rt can be hex or decimal 
            // get decimal, sign extend to 32, add to register
             if (Dec_int != 0){
                
                int Signed_int = signExtension(Dec_int);
                registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))] + Signed_int;
            } else{
                registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))] + signExtension(atoi(array[3]));
            }

            
        }

        if (strcmp(op, "andi") == 0){
               if (Dec_int != 0){
               
                
                int Signed_int = zeroExtension(Dec_int);
                registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))] & Signed_int;
            } else{
                registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))] & zeroExtension(atoi(array[3]));
            }

        }
        
        if (strcmp(op, "ori") == 0){
            if (Dec_int != 0){
                
                
                int Signed_int = zeroExtension(Dec_int);
                registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))] | Signed_int;
            } else{
                registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))] | zeroExtension(atoi(array[3]));
            }
        }

        if (strcmp(op, "xori") == 0){
           if (Dec_int != 0){
              
                
                int Signed_int = zeroExtension(Dec_int);
                registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))] ^ Signed_int;
            } else{
                registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))] ^ zeroExtension(atoi(array[3]));
            }
        }

        if (strcmp(op, "slti") == 0){
            if (Dec_int != 0){
                
                
                int Signed_int = signExtension(Dec_int);
                if( registers->r[atoi(strtok(array[2], "$"))] < Signed_int){
                    registers->r[atoi(strtok(array[1], "$"))] = 1;
                } else{
                    registers->r[atoi(strtok(array[1], "$"))] = 0;
                }
            } else{
                if( registers->r[atoi(strtok(array[2], "$"))] < signExtension(atoi(array[3]))){
                    registers->r[atoi(strtok(array[1], "$"))] = 1;
                } else {
                    registers->r[atoi(strtok(array[1], "$"))] = 0;
                }
            }
        }
	if (strcmp(op, "sll") == 0){
	 if (Dec_int != 0){
	    
	    registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))]<<Dec_int;
	  } else {
	    registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))]<<(atoi(array[3]));
	    
	  }
	}
	if (strcmp(op, "sra") == 0){
	  if (Dec_int != 0){
	    
	    registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))]>>Dec_int;
	  } else {
	    registers->r[atoi(strtok(array[1], "$"))] = registers->r[atoi(strtok(array[2], "$"))]>>(atoi(array[3]));
	    
	  }
	}
	if (strcmp(op, "lui") == 0){
	  int twoToTheSixteen = 65536;
	  if (array_2 != 0){
	    
	    registers->r[atoi(strtok(array[1], "$"))] = array_2*twoToTheSixteen;
	  } else {
	    int shifted_immediate = atoi(array[2])*twoToTheSixteen;
	    registers->r[atoi(strtok(array[1], "$"))] = shifted_immediate;
	  }
	}
	

    }
    else if (op_type == 2){
      if (strcmp(op, "lb") == 0){
	 if (array_2 != 0){
	  
	  int address = array_2 + registers->r[atoi(strtok(array[3], "$"))];
	  int fetched_byte = (ht_get(table, address));
if ((fetched_byte & 0x80) == 0x80) { // sign extend 8-bit -> 32-bit
            fetched_byte |= 0xffffff00;
          }
          
	  registers->r[atoi(strtok(array[1], "$"))] = fetched_byte;
	  
	}
	else{
	  int address = (atoi(array[2])) + registers->r[atoi(strtok(array[3], "$"))];
	  int fetched_byte = (ht_get(table, address));
	  if ((fetched_byte & 0x80) == 0x80) { // sign extend 8-bit -> 32-bit
            fetched_byte |= 0xffffff00;
	  }
	  registers->r[atoi(strtok(array[1], "$"))] = fetched_byte;
	}      
      }

      if (strcmp(op, "lbu") == 0){
	 if (array_2 != 0){
	 
	  int address = array_2+ registers->r[atoi(strtok(array[3], "$"))];
	  int fetched_byte = zeroExtension(ht_get(table, address));
	  registers->r[atoi(strtok(array[1], "$"))] = fetched_byte;
	  
	}
	else{
	  int address = (atoi(array[2])) + registers->r[atoi(strtok(array[3], "$"))];
	  int fetched_byte = zeroExtension(ht_get(table, address));
	  registers->r[atoi(strtok(array[1], "$"))] = fetched_byte;
	}      
      }
      
      if (strcmp(op, "sb") == 0){
	 if (array_2 != 0){
	  
	  int address = array_2 + registers->r[atoi(strtok(array[3], "$"))];
	  if(address%4 == 0){

	  }
	  if(address%4 == 1){

	  }
	  int word_containing_byte_to_store = registers->r[atoi(strtok(array[1], "$"))]; 
	  int byte_to_store = word_containing_byte_to_store&255;
	  ht_add(table, address, byte_to_store);
	  
	}
	else{
	  int address = (atoi(array[2])) + registers->r[atoi(strtok(array[3], "$"))];
	  int word_containing_byte_to_store = registers->r[atoi(strtok(array[1], "$"))]; 
	  int byte_to_store = word_containing_byte_to_store&255;
	  ht_add(table, address, byte_to_store);
	}      
      }

      if (strcmp(op, "lw") == 0){
	 if (array_2 != 0){
	  
	  int address = array_2 + registers->r[atoi(strtok(array[3], "$"))];
	  int byte1 = ht_get(table, address);
	  int byte2 = ht_get(table, address+1)<<8;
	  int byte3 = ht_get(table, address+2)<<16;
	  int byte4 = ht_get(table, address+3)<<24;
	  int word_to_load = byte1 | byte2 | byte3 | byte4;
	  registers->r[atoi(strtok(array[1], "$"))] = word_to_load;
	} else {
	    int address = (atoi(array[2])) + registers->r[atoi(strtok(array[3], "$"))];
	    int byte1 = ht_get(table, address);
     	    int byte2 = ht_get(table, address+1)<<8;
     	    int byte3 = ht_get(table, address+2)<<16;
	    int byte4 = ht_get(table, address+3)<<24;
	    int word_to_load = byte1 | byte2 | byte3 | byte4;
	    registers->r[atoi(strtok(array[1], "$"))] = word_to_load;
	}
      }
      
      if (strcmp(op, "sw") == 0){
	 if (array_2 != 0){
	  
	  int address = array_2 + registers->r[atoi(strtok(array[3], "$"))];
	  int word_to_store = registers->r[atoi(strtok(array[1], "$"))];
	  int byte1 = word_to_store & 255;
	  int byte2 = word_to_store & 65280;
	  byte2 = byte2 >> 8;
	  int byte3 = word_to_store & 16711680;
	  byte3 = byte3 >> 16;
	  int byte4 = word_to_store & 4278190080;
	  byte4 = byte4 >> 24;
	  ht_add(table, address, byte1);
	  ht_add(table, address+1, byte2);
	  ht_add(table, address+2, byte3);
	  ht_add(table, address+3, byte4);
	} else {
	  int address = (atoi(array[2])) + registers->r[atoi(strtok(array[3], "$"))];
	  int word_to_store = registers->r[atoi(strtok(array[1], "$"))];
	  int byte1 = word_to_store & 255;
	  int byte2 = word_to_store & 65280;
	  byte2 = byte2 >> 8;
	  int byte3 = word_to_store & 16711680;
	  byte3 = byte3 >> 16;
	  int byte4 = word_to_store & 4278190080;
	  byte4 = byte4 >> 24;
	  ht_add(table, address, byte1);
	  ht_add(table, address+1, byte2);
	  ht_add(table, address+2, byte3);
	  ht_add(table, address+3, byte4);

	}
      }


      
    }

}


        


    


    
    // TODO: write logic for evaluating instruction on current interpreter state
