/**
 * @author John Abo
 * Student ID: 30088517
 *
 * Just some class I made to test the code independent of the actually
 * MinHeap.java file
 */

package ca.ucalgary.cpsc331;
import ca.ucalgary.cpsc331.PriorityQueue;

public class Tester {
	
	//I used this function to test a specific set of numbers
	private static void initTest(int min, int max, int size, MinHeap target) {
		for (int i = size; i >= 0 ; i--) {
			target.insert((int)Math.floor(Math.random()*(max-min+1)+min));
		}			
	}
	
	public static void main(String[] arg) {
		System.out.println("Testing:\n");
		
		MinHeap subject01 = new MinHeap();
		MinHeap subject02 = new MinHeap();
		MinHeap subject03 = new MinHeap();
		
		System.out.println("\nInitializing\n");
		
		initTest(-30, 30, 20, subject01);
		initTest(0, 30, 30, subject02);
		
		//Attempts to insert excess
		try {
			initTest(-9999, 9999, 34, subject03);
		} catch (RuntimeException e) {
			System.out.println(e.getMessage());
		}
		//System.out.println("\nHeapifying\n");
		
		//I shouldn't need this, since insert should maintain min-heap properties
		// try {
			// subject01.buildMinHeap();
		// } catch (Exception e) {
			// System.out.println("failed to build heap from array 1");
			// System.out.println(e.getMessage());
		// }
		
		// try {
			// subject02.buildMinHeap();
		// } catch (Exception e) {
			// System.out.println("failed to build heap from array 2");
			// System.out.println(e.getMessage());
		// }
		
		System.out.println("\nCurrent state:\n");
		
		//Test prints
		System.out.println("\nSubject 01:\n" + subject01.toString());
		System.out.println("\nSubject 02:\n" + subject02.toString());
		
		
		System.out.println("\nTesting extract min:\n");
		
		//Extracts 2 keys from subject01 and 1 key from subject02
		//now also extracts 32 keys from subject03, the last extraction should
		//fail and print a message
		int test = subject01.extractMin();
		System.out.println("Test extract: " + test);
		
		test = subject01.extractMin();
		System.out.println("Test extract: " + test);
		
		test = subject02.extractMin();
		System.out.println("Test extract: " + test);
		
		System.out.println("\nCurrent state:\n");
		
		//Test prints
		System.out.println("\nSubject 01:\n" + subject01.toString());
		System.out.println("\nSubject 02:\n" + subject02.toString());
	}
}