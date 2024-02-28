import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Client {
    private static final String SERVER_ADDRESS = "127.0.0.1";
    private static final short SERVER_PORT = 1234;

    public static void main(String[] args) {
        try(Socket client = new Socket(SERVER_ADDRESS, SERVER_PORT)) {
            // out/in streams of the client socket
            PrintWriter clientOut = new PrintWriter(client.getOutputStream(), true);
            BufferedReader clientIn = new BufferedReader(new InputStreamReader(client.getInputStream()));

            // reader for console
            BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

            // reading the size of the matrix
            // ensuring that the size is correct at the start of the game
            int n = 0;
            do {
                System.out.print("[CLIENT] Dati numar din [4, 9]: ");
                String input = reader.readLine();
                try {
                    Integer.parseInt(input.trim());
                } catch(NumberFormatException nFE) {
                    System.out.println("[CLIENT] Nu am primit un numar!");
                }

                n = Integer.parseInt(input.trim());
                if (n < 4 || n > 9) {
                    System.out.println("[CLIENT] Numar primit invalid!");
                }
            } while (n < 4 || n > 9);

            // sending the size of the matrix
            System.out.println("[CLIENT] Sending n...");
            clientOut.println(n);
            String raspuns = clientIn.readLine();
            System.out.println("[CLIENT] " + raspuns);

            // conclusion of the game
            String concluzie = "";

            // conclusion shall be won/lost at the end of the game
            while(!concluzie.equals("Joc castigat!") || !concluzie.equals("Joc pierdut!")) {
                // reading a line
                System.out.print("[CLIENT] Dati o linie: ");
                String inputLinie = reader.readLine();
                try {
                    // checking if the line read is really a number
                    Integer.parseInt(inputLinie.trim());
                } catch(NumberFormatException nFE) {
                    System.out.println("[CLIENT] Nu am primit un numar!");
                    continue;
                }

                // reading a column
                System.out.print("[CLIENT] Dati o coloana: ");
                String inputColoana = reader.readLine();
                try {
                    // checking if the column read is really a number
                    Integer.parseInt(inputColoana.trim());
                } catch(NumberFormatException nFE) {
                    System.out.println("[CLIENT] Nu am primit un numar!");
                    continue;
                }
                
                // converting input line/column to number
                int linie = Integer.valueOf(inputLinie.trim());
                int coloana = Integer.valueOf(inputColoana.trim());

                System.out.println("[CLIENT] Sending position to the server: " + linie + " " + coloana);
                // sending the position to the server
                clientOut.println(Integer.toString(linie));
                clientOut.println(Integer.toString(coloana));

                // reading the conclusion of the try
                concluzie = clientIn.readLine();
                int greseli = Integer.parseInt(clientIn.readLine().trim());
                int numarAvioane = Integer.parseInt(clientIn.readLine().trim());

                System.out.println("\n[CLIENT] Concluzie bombardament" + concluzie);
                System.out.println("[CLIENT] Numar greseli: " + greseli);
                System.out.println("[CLIENT] Numar avioane nebombardate: " + numarAvioane);
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}