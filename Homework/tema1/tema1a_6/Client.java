package tema1.tema1a_6;

import java.net.*;
import java.io.IOException;
import java.io.*;
import java.util.ArrayList;

public class Client {
    private static final String SERVER_ADDRESS = "127.0.0.1";
    private static final int SERVER_PORT = 1234;

    private static void closeStreams(Socket socket, BufferedReader reader) throws IOException {
        if(socket != null) {
            try {
                socket.close();
            } catch (IOException iOE) {
                System.err.println(iOE.getMessage());
            }
        }

        if (reader != null) {
            try {
                reader.close();
            } catch (IOException iOE) {
                System.err.println(iOE.getMessage());
            }
        }
    }

    public static void main(String[] args) throws IOException {
        Socket socket = null;
        try {
            socket = new Socket(SERVER_ADDRESS, SERVER_PORT);
        } catch (IOException iOE) {
            System.err.println(iOE.getMessage());
        }

        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        
        System.out.print("[CLIENT] Read character: ");
        String charBuffer = reader.readLine();
        if(charBuffer.length() > 1 || charBuffer.length() == 0) {
            System.err.println("error on reading.");   
            closeStreams(socket, reader);
        }
        char character = charBuffer.charAt(0);
    
        System.out.print("[CLIENT] Read string: ");
        String string = reader.readLine();

        DataOutputStream socketOut = new DataOutputStream(socket.getOutputStream());
        socketOut.writeChar(character);
        socketOut.writeInt(string.length());
        socketOut.writeChars(string);
        socketOut.flush();

        DataInputStream socketIn = new DataInputStream(socket.getInputStream());
        int resultSize = socketIn.readInt();
        ArrayList<Integer> result = new ArrayList<>(resultSize);

        for(int i = 0; i < resultSize; i++) {
            result.add(socketIn.readInt());
        }

        System.out.print("[CLIENT] Positions: ");
        result.forEach(item -> System.out.println(item + " "));

        closeStreams(socket, reader);
    }
}
