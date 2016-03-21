import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Scanner;

public class Main {

	public Main() {

	}

	/**
	 * Comparator pentru sortarea la scriere
	 */
	static class MyComparator implements Comparator<LinkedList<Solution>> {
		//indexul fiecarui document in fisierul de intrare
		HashMap<String, Integer> nameIndex;

		/**
		 * 
		 * @param documentNames numele documentelor in ordinea in care apar in
		 * fisierul de intrare
		 */
		public MyComparator(String documentNames[]) {
			nameIndex = new HashMap<String, Integer>();

			for (int i = 0; i < documentNames.length; i++)
				nameIndex.put(documentNames[i], i);
		}

		/**
		 * @param o1 lista rezultata in urma operatiei de reduce
		 * @param o2 lista rezultata in urma operatiei de reduce
		 */
		@Override
		public int compare(LinkedList<Solution> o1, LinkedList<Solution> o2) {
			Solution s1, s2;
			s1 = o1.getFirst();
			s2 = o2.getFirst();

			if (Math.abs(s1.rank - s2.rank) < 0.009) {
				// rankuri egale
				return Integer.compare(nameIndex.get(s1.fileName), nameIndex.get(s2.fileName));
			} else
				//ordine invera a rankurilor
				return -Float.compare(s1.rank, s2.rank);
		}
	}

	public static void main(String[] args) throws IOException, InterruptedException {
		int threads, fragmentSize, documents, fileSize;
		String documentNames[];
		
		//reader pentru fisierele din input/output
		FileReader reader;
		FileWriter writer;
		Scanner readerScanner; ///scanner pentru input
		
		//workpool pentru map si pentru reduce
		WorkPool mapWorkPool;
		WorkPool reduceWorkPool;
		Worker workers[]; //workeri
		
		//solutii map/reduce
		ArrayList<LinkedList<Solution>> solutions;

		if (args.length != 3) {
			System.out.println("Usage: java -jar mapreduce.jar $NT $INPUTFILE $OUTPUTFILE");
			return;
		}

		threads = Integer.parseInt(args[0]);
		reader = new FileReader(args[1]);
		writer = new FileWriter(args[2]);
		readerScanner = new Scanner(reader);

		mapWorkPool = new WorkPool(threads);
		reduceWorkPool = new WorkPool(threads);
		workers = new Worker[threads];

		for (int i = 0; i < threads; i++)
			workers[i] = new Worker(mapWorkPool);

		fragmentSize = readerScanner.nextInt();
		documents = readerScanner.nextInt();
		readerScanner.nextLine();
		solutions = new ArrayList<LinkedList<Solution>>();
		documentNames = new String[documents];

		//genereaza taskuri map
		for (int i = 0; i < documents; i++) {
			documentNames[i] = readerScanner.nextLine();
			fileSize = (int) new File(documentNames[i]).length();

			//lista pentru solutiile map ale acestui fisier
			solutions.add(new LinkedList<Solution>());

			for (int j = 0; j < fileSize / fragmentSize; j++) {
				mapWorkPool.putWork(new MapPartialSolution(documentNames[i], j * fragmentSize,
						fragmentSize, solutions.get(i)));
			}

			//ultimul fragment
			if (fileSize % fragmentSize != 0)
				mapWorkPool.putWork(new MapPartialSolution(documentNames[i], fileSize - (fileSize % fragmentSize),
						fileSize % fragmentSize, solutions.get(i)));
		}

		for (int i = 0; i < threads; i++)
			workers[i].start();

		//asteapta terminarea map-ului
		synchronized (mapWorkPool) {
			while (!mapWorkPool.ready)
				mapWorkPool.wait();
		}

		//taskuri reduce
		for (int i = 0; i < documents; i++) {
			reduceWorkPool.putWork(new ReducePartialSolution(documentNames[i], solutions.get(i)));
		}

		for (int i = 0; i < threads; i++) {
			workers[i] = new Worker(reduceWorkPool);
			workers[i].start();
		}

		//asteapta terminarea reduce-ului
		synchronized (reduceWorkPool) {
			while (!reduceWorkPool.ready)
				reduceWorkPool.wait();
		}

		//sorteaza output
		Collections.sort(solutions, new MyComparator(documentNames));

		for (int i = 0; i < documents; i++) {
			Solution solution = solutions.get(i).getFirst();
			String line = String.format("%s;%.2f;[%d,%d]\n", solution.fileName, solution.rank, solution.maximalLength,
					solution.maximalWords.size());
			writer.write(line);
		}

		readerScanner.close();
		reader.close();
		writer.close();
	}
}
