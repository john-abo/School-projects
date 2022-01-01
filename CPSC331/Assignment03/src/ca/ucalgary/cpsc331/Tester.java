package ca.ucalgary.cpsc331;

import ca.ucalgary.cpsc331.HashTable;

public class Tester {
	public static void main (String[] args) {
		System.out.println("Bring on the pain");
		System.out.println("We are abandoned, to wallow in our own filth");
		System.out.println("\nInitializing");
		
		HashTable subject1 = new HashTable();
		HashTable subject2 = new HashTable();
		HashTable subject3 = new HashTable();
		
		System.out.println("\nTesting empty() and full() methods\n");
		System.out.println("Expected: false, Actual: " + subject1.full());
		System.out.println("Expected: true, Actual: " + subject1.empty());
		System.out.println("Expected: true, Actual: " + subject2.empty());
		System.out.println("Expected: true, Actual: " + subject3.empty());
		
		System.out.println("\nCurrent state:\n");
		System.out.println("Subject 1: \n\"\n" + subject1.toString() + "\n\"\n");
		System.out.println("Subject 2: \n\"\n" + subject2.toString() + "\n\"\n");
		System.out.println("Subject 3: \n\"\n" + subject3.toString() + "\n\"\n");
		
		System.out.println("\nFilling tables...\n");
		
		for (int i = 0; i < 5; i++) {
			subject1.insert(Integer.toString(i));
		}
		
		for (int i = 0; i < 17; i++) {
			subject2.insert(Integer.toString(i));
		}
		
		System.out.println("\nCurrent state:\n");
		System.out.println("Subject 1: \n\"\n" + subject1.toString() + "\n\"\n");
		System.out.println("Subject 2: \n\"\n" + subject2.toString() + "\n\"\n");
		System.out.println("Subject 3: \n\"\n" + subject3.toString() + "\n\"\n");
		
		System.out.println("\nTesting empty() and full() methods\n");
		System.out.println("Expected: false, Actual: " + subject1.full());
		System.out.println("Expected: true, Actual: " + subject2.full());
		System.out.println("Expected: false, Actual: " + subject1.empty());
		System.out.println("Expected: false, Actual: " + subject2.empty());
		System.out.println("Expected: true, Actual: " + subject3.empty());
		
		System.out.println("\nAttempting to insert to full table\n");
		try {
			subject2.insert(Integer.toString(4654));
		} catch (RuntimeException e) {
			System.out.println("This should print with error message:\n" + e.getMessage());
		}
		
		System.out.println("\nCurrent state:\n");
		System.out.println("Subject 1: \n\"\n" + subject1.toString() + "\n\"\n");
		System.out.println("Subject 2: \n\"\n" + subject2.toString() + "\n\"\n");
		System.out.println("Subject 3: \n\"\n" + subject3.toString() + "\n\"\n");
		
		System.out.println("\nDeleting characters in Table\n");
		
		for (int i = 3; i < 5; i++) {
			subject1.delete(Integer.toString(i));
			subject2.delete(Integer.toString(i));
		}
		
		for (int i = 10; i < 15; i++) {
			subject2.delete(Integer.toString(i));
		}
		
		System.out.println("\nCurrent state:\n");
		System.out.println("Subject 1: \n\"\n" + subject1.toString() + "\n\"\n");
		System.out.println("Subject 2: \n\"\n" + subject2.toString() + "\n\"\n");
		System.out.println("Subject 3: \n\"\n" + subject3.toString() + "\n\"\n");
		
		System.out.println("\nAttempting to delete from empty table and table with out key\n");
		
		try {
			subject3.delete("Test");
		} catch (RuntimeException e) {
			System.out.println("Failed to delete from empty table, with message:\n" + e.getMessage());
		}
		
		try {
			subject2.delete("Test");
		} catch (RuntimeException e) {
			System.out.println("Failed to delete key that isn't in table:\n" + e.getMessage());
		}
		
		System.out.println("\nEnding...");
	}
	
	private static void printContent(HashTable subject) {
		System.out.println("Table and marker contents");
		
		String[] test1 = subject.getTable();
		String[] test2 = subject.getMarker();
		
		for (int i = 0; i < 17; i++) {
			if (test1[i] == null) {
				System.out.println("null:" + test2[i]);
			} else {
				System.out.println(test1[i] + ":" + test2[i]);
			}
		}
	}
}
