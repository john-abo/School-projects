package ca.ucalgary.cpsc331;
import ca.ucalgary.cpsc331.Dictionary;

public class Tester {
	
	public static void main(String[] args) {
		System.out.println("Testing RedBlackTree:");
		
		RedBlackTree subject1 = new RedBlackTree();
		RedBlackTree subject2 = new RedBlackTree();
		RedBlackTree subject3 = new RedBlackTree();
		
		subject1.insert(1);
		System.out.println("\nExpected: true, Actual: " + subject2.empty());
		System.out.println("\nExpected: false, Actual: " + subject1.empty());
		
		System.out.println("\nCurrent state:");
		System.out.println("\nSubject 1: \n" + subject1.toString());
		System.out.println("\nSubject 2: \n" + subject2.toString());
		System.out.println("\nSubject 3: \n" + subject3.toString());
		
		System.out.println("Inserting for subject1");
		
		subject1.insert(2);
		subject1.insert(3);
		subject1.insert(4);
		subject1.insert(5);
		subject1.insert(6);
		
		System.out.println("Inserting for subject2");
		
		subject2.insert(100);
		subject2.insert(23);
		subject2.insert(5);
		subject2.insert(5);
		subject2.insert(56);
		subject2.insert(123413);
		
		System.out.println("Inserting for subject3");
		
		subject3.insert(-1);
		subject3.insert(-2);
		subject3.insert(-33);
		
		System.out.println("\nCurrent state:");
		System.out.println("\nSubject 1: \n" + subject1.toString());
		System.out.println("\nSubject 2: \n" + subject2.toString());
		System.out.println("\nSubject 3: \n" + subject3.toString());
		
		System.out.println("Deleting for subject1");
		
		subject1.delete(3);
		subject1.delete(4);
		
		System.out.println("Deleting for subject2");
		
		subject2.delete(123413);
		
		System.out.println("Deleting for subject3");
		
		try {
			subject3.delete(10);
		} catch(RuntimeException e) {
			System.out.println("This should print with error message:\n" + e.getMessage());
		}
		
		System.out.println("\nCurrent state:");
		System.out.println("\nSubject 1: \n" + subject1.toString());
		System.out.println("\nSubject 2: \n" + subject2.toString());
		System.out.println("\nSubject 3: \n" + subject3.toString());
		
		System.out.println("Checking member ship of keys:");
		
		System.out.println("\nExpected: true, Actual: " + subject3.member(-33));
		System.out.println("\nExpected: true, Actual: " + subject2.member(100));
		System.out.println("\nExpected: true, Actual: " + subject1.member(5));
		System.out.println("\nExpected: false, Actual: " + subject3.member(0));
		System.out.println("\nExpected: false, Actual: " + subject2.member(0));
		System.out.println("\nExpected: false, Actual: " + subject1.member(0));
		
		System.out.println("\nLARGE TREE TEST:\n");
		
		RedBlackTree subject4 = new RedBlackTree();
		subject4.insert(2);
		subject4.insert(3);
		subject4.insert(4);
		subject4.insert(5);
		subject4.insert(6);
		subject4.insert(100);
		subject4.insert(23);		//this line is problematic
		subject4.insert(5);
		subject4.insert(5);
		subject4.insert(56);
		subject4.insert(1234);
		subject4.insert(-1);
		subject4.insert(-2);
		subject4.insert(-33);
		
		System.out.println("\nSubject 4: \n" + subject4.toString());
	}
}