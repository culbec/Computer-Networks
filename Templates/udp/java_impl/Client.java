package udp.java_impl;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class Client {
    private static final String SERVER_ADDRESS = "localhost";
    private static final short SERVER_PORT = 1234;

    public static void main(String[] args) {
        try(DatagramSocket client = new DatagramSocket()) {
            try(BufferedReader reader = new BufferedReader(new InputStreamReader(System.in))) {
                System.out.print("Read str: ");
                String string = reader.readLine();

                // address where we want to send/recv data
                InetAddress ipAddress = InetAddress.getByName(SERVER_ADDRESS);

                // array of bytes that contain our data
                byte[] recvData = new byte[1024];
                byte[] sendData = new byte[1024];
                
                // new datagram packet that contains the data that we want to send/recv
                DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, ipAddress, SERVER_PORT);
                DatagramPacket recvPacket = new DatagramPacket(recvData, recvData.length, ipAddress, SERVER_PORT);

                sendPacket.setData(string.getBytes());
                sendPacket.setLength(string.length());
                
                // sending the packet to the server
                client.send(sendPacket);

                // receiving the packet from the server
                client.receive(recvPacket);

                String data = new String(recvPacket.getData());
                System.out.println(data);

            } catch (IOException iOE) {
                System.err.println(iOE.getMessage());
            }
        } catch (IOException iOE) {
            System.err.println(iOE.getMessage());
        }
    }
}
