package ca.ucalgary.cpsc331;

import ca.ucalgary.cpsc331.Dictionary;

public class HashTable {
	
	private String[] table;
	private String[] marker;
	private final int CAPACITY = 17;
	
	public HashTable() {
		table = new String[CAPACITY];
		marker = new String[CAPACITY];
		
		for (int i = 0; i < CAPACITY; i++) {
			marker[i] = "nil";
		}
	}
	
	public boolean empty(){
		
		for (int i = 0; i < CAPACITY; i++) {
			if (marker[i].equalsIgnoreCase("occ")) {
				return false;
			}
		}
		
		return true;
	}
	
    public boolean full(){
		
		for (int i = 0; i < CAPACITY; i++) {
			if (marker[i].equalsIgnoreCase("nil") || marker[i].equalsIgnoreCase("deleted")) {
				return false;
			}
		}
		
		return true;
	}
	
    public void insert(String key){
		int i = 0;
		int j;
		
		do {
			
			j = hash(key, i);
			
			if (marker[j].equalsIgnoreCase("nil") || marker[j].equalsIgnoreCase("deleted")) {
				table[j] = key;
				marker[j] = "occ";
				return;
			} else i++;
			
		} while (i < CAPACITY);
		
		throw new RuntimeException("Could not insert key \"" + key + "\" into full table");
	}
	
    public void delete(String key){
		int i = 0;
		int j;
		
		if (empty()) {
			throw new RuntimeException("Table is empty");
		}
		
		do {
			j = hash(key, i);
			
			if (table[j].equalsIgnoreCase(key)) {
				marker[j] = "delete";
				return;
			}
			else i++;
			
			//System.out.println("j: " + j + "\ni: " + i);
			
		} while(table[j] != null && i < CAPACITY);
		
		throw new RuntimeException("String \"" + key + "\" was not found in the table");
	}
	
    public boolean member(String key){
		
		int i = 0;
		int j;
		
		do {
			j = hash(key, i);
			
			if (table[j].equalsIgnoreCase(key)) return true;
			else i++;
			
		} while (table[j] != null && i < CAPACITY);
		
		return false;
	}
	
	/**
	 * Hash function that will be used in the table. Makes use of the
	 * built in hashing function for strings.
	 */
	private int hash(String k, int i) {
		return (k.hashCode() + i) % CAPACITY;
	}
	
	@Override
	public String toString() {
		String ret = "";
		
		if (!empty()) {
			for (int i = 0; i < CAPACITY; i++) {
			
				if (!marker[i].equalsIgnoreCase("nil")) {
					ret += i + ":";
				
					if (marker[i].equalsIgnoreCase("delete")) {
						ret += "deleted";
					} else {
						ret += "\"" + table[i] + "\"";
					}
				
					ret += "\n";
				}	
			}
			ret = ret.substring(0, ret.length() - 1);
		}
		
		return ret;
	}
	
	//Getters made out of habbit at this point
	
	public String[] getTable() {
		return table;
	}
	
	public String[] getMarker() {
		return marker;
	}
}