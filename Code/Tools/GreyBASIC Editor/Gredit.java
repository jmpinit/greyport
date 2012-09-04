import java.util.*;
import java.io.*;

public class Gredit
{
	static List<Integer> program = new ArrayList<Integer>();
	
	public static void main(String[] args)
	{
		if(args[0].length()==0)
		{
			System.out.println("Must specify program file.");
			System.exit(0);
		}
		
		String text = "";
		try{		
			//set up file interface
			FileInputStream file = new FileInputStream(args[0]);
			
			//read file
			int length = file.available();
			byte[] data = new byte[length];
			file.read(data);
			
			file.close();
			
			text = new String(data);
		} catch(Exception e) {
			System.out.println(e.toString());
		}
		
		String[] lines = text.split("\n");
		
		for(int i=0; i<lines.length; i++) {
			String line = lines[i].trim();
			
			if(line.startsWith("GOTO"))
			{
				program.add(1);
				
				String[] parts = line.split(" ");
				if(parts[1].length()!=1)
				{
					error(i, "Variable should be a single letter.");
				} else {
					char var = parts[1].charAt(0);
					if(var>='A'&&var<='Z')
					{
						var -= 'A';
						program.add((int)var);
					} else {
						error(i, "Variable should be a letter.");
					}
				}
			} else if(line.startsWith("SET"))
			{
				program.add(2);
				
				String[] parts = line.split(" ");
				
				if(parts.length!=3)
				{
					error(i, "Wrong number of parameters for SET.");
				} else {
					if(parts[1].length()!=1)
					{
						error(i, "Variable should be a single letter.");
					} else {
						char var = parts[1].charAt(0);
						if(var>='A'&&var<='Z')
						{
							var -= 'A';
							program.add((int)var);
							
							int value = Integer.parseInt(parts[2]);
							int high = value>>8;
							int low = 0x00FF&value;
							program.add(high);
							program.add(low);
						} else {
							error(i, "Variable is not a letter.");
						}
					}
				}
			} else if(line.startsWith("ADD"))
			{
				program.add(3);
				
				String[] parts = line.split(" ");
				if(parts.length!=3)
				{
					error(i, "Wrong number of parameters for ADD.");
				} else {
					char var = parts[1].charAt(0);
					if(var>='A'&&var<='Z')
					{
						var -= 'A';
						program.add((int)var);
					} else {
						error(i, "Variable is not a letter.");
					}
					
					var = parts[2].charAt(0);
					if(var>='A'&&var<='Z')
					{
						var -= 'A';
						program.add((int)var);
					} else {
						error(i, "Variable is not a letter.");
					}
				}
			} else if(line.startsWith("SUB"))
			{
				program.add(4);
				
				String[] parts = line.split(" ");
				if(parts.length!=3)
				{
					error(i, "Wrong number of parameters for ADD.");
				} else {
					char var = parts[1].charAt(0);
					if(var>='A'&&var<='Z')
					{
						var -= 'A';
						program.add((int)var);
					} else {
						error(i, "Variable is not a letter.");
					}
					
					var = parts[2].charAt(0);
					if(var>='A'&&var<='Z')
					{
						var -= 'A';
						program.add((int)var);
					} else {
						error(i, "Variable is not a letter.");
					}
				}
			} else if(line.startsWith("MULT"))
			{
				program.add(5);
				
				String[] parts = line.split(" ");
				if(parts.length!=3)
				{
					error(i, "Wrong number of parameters for ADD.");
				} else {
					char var = parts[1].charAt(0);
					if(var>='A'&&var<='Z')
					{
						var -= 'A';
						program.add((int)var);
					} else {
						error(i, "Variable is not a letter.");
					}
					
					var = parts[2].charAt(0);
					if(var>='A'&&var<='Z')
					{
						var -= 'A';
						program.add((int)var);
					} else {
						error(i, "Variable is not a letter.");
					}
				}
			} else if(line.startsWith("DIV"))
			{
				program.add(6);
				
				String[] parts = line.split(" ");
				if(parts.length!=3)
				{
					error(i, "Wrong number of parameters for ADD.");
				} else {
					char var = parts[1].charAt(0);
					if(var>='A'&&var<='Z')
					{
						var -= 'A';
						program.add((int)var);
					} else {
						error(i, "Variable is not a letter.");
					}
					
					var = parts[2].charAt(0);
					if(var>='A'&&var<='Z')
					{
						var -= 'A';
						program.add((int)var);
					} else {
						error(i, "Variable is not a letter.");
					}
				}
			} else if(line.startsWith("SKIP_IF"))
			{
				String parts[] = line.split(" ");
				
				if(parts.length!=4)
				{
					error(i, "An IF statement needs two var parameters and an operator.");
				} else {
					if(parts[2].equals("="))
					{
						program.add(7);
					} else if(parts[2].equals(">"))
					{
						program.add(8);
					} else if(parts[2].equals("<"))
					{
						program.add(9);
					} else if(parts[2].equals(">="))
					{
						program.add(10);
					} else if(parts[2].equals("<="))
					{
						program.add(11);
					} else {
						error(i, "Weird operator for IF statement.");
					}
					
					char var = parts[1].charAt(0);
					if(var>='A'&&var<='Z')
					{
						var -= 'A';
						program.add((int)var);
					} else {
						error(i, "First variable is not a letter.");
					}
					
					var = parts[3].charAt(0);
					if(var>='A'&&var<='Z')
					{
						var -= 'A';
						program.add((int)var);
					} else {
						error(i, "Second variable is not a letter.");
					}
				}
			} else if(line.startsWith("DELAY"))
			{
				program.add(12);
				program.add(0);
				
				String[] parts = line.split(" ");
				
				char var = parts[1].charAt(0);
				if(var>='A'&&var<='Z')
				{
					var -= 'A';
					program.add((int)var);
				} else {
					error(i, "Variable is not a letter.");
				}
			} else if(line.startsWith("PRINT"))
			{
				program.add(12);
				program.add(1);
				
				String[] parts = line.split(" ");
				program.add((int)parts[1].charAt(0));
			} else if(line.startsWith("CLEAR"))
			{
				program.add(12);
				program.add(2);
			} else if(line.startsWith("EXIT"))
			{
				program.add(255);
			} else {
				error(i, "Invalid line start.");
			}
		}
		
		for(int i=0; i<program.size(); i++)
		{
			System.out.print(program.get(i)+", ");
		}
	}
	
	public static void error(int line, String msg)
	{
		System.out.println("At line "+line+", error: "+msg);
		System.exit(0);
	}
}