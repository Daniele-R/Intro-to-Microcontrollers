program NumberCounter;

    #include( "stdlib.hhf" )

static
    memPointer:     int32;                                //Must be 32 in order to record addresses
    number:         int16;                                //Numbers are capped to 16 bits
    count:          uns8 := 0;                            //It will count numbers up to 8 bits

begin NumberCounter;

    stdout.put("Project 3 - Daniele Ricciardelli",nl);

    mem.alloc (1000);                                //Allocate 1000 bits
    MOV (EAX, memPointer);                            //Save allocation address for releasing
    
    stdout.put ("Enter 0 to stop: ");                //User input
    stdin.get (number);
    
    while (number != 0) do                            //If user types 0, the while loop ends
        
        INC (count);                                //INC = Increament variable by 1
        MOV (number, [EAX]);                        //Moves number to EAX
        ADD (2, EAX);                                //Moves to next available space
        
        stdout.put ("Enter 0 to stop: ");            //User input loop
        stdin.get (number);
    endwhile;
    
    MOV(memPointer, EAX);                            //Moves EAX back to the the beginning
    
    for (MOV(0,CL); CL < count; INC(CL)) do            //For loop to print as many numbers as count recorded
        
        MOV ([EAX], number);                        //Move the numbers given into the variable number
        stdout.put(number, " ");                    //1 spacebar between each character
        ADD (2,EAX);                                //Moves into the next available space
    endfor;
    
    MOV(memPointer, EAX);                            //End of the memory and releasing info on EAX
    mem.free (EAX);
    
    stdout.put(nl,"Daniele Ricciardelli - End of Project 3");
    
end NumberCounter;
