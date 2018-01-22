/*
    Rob-Fan
    Raghib Mahtab Q
    October 2017

    An Android app that allows a user to control two servos in order to adjust the position of a fan.
    Uses a Bluetooth module (HC-05) to interface between the phone and the board.
    Will eventually become the IoT monitor of an autonomous fan.

    Current problem:
    -The app doesn't crash, but pressing buttons does nothing to affect the servos
    -The problem is either in:
    -The connection (is it properly being created?),
    -Sending data (I'm probably doing it wrong),
    -Or reading and responding to it in the Arduino sketch

    Next steps:
    -Fix above problem
    -Send operation data over to the app from the Arduino
 */

package io.robotitect.robfan;

import android.os.ParcelUuid;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.NoSuchElementException;
import java.util.Set;

import android.widget.TextView;
import android.widget.Toast;
import android.content.Intent;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

public class MainActivity extends AppCompatActivity
{
    private final String DEVICE_NAME="HC-05";
//    private final UUID PORT_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
    private BluetoothAdapter myBluetooth = null;
    private Set pairedDevices;
    private BluetoothDevice device;
    private BluetoothAdapter bluetoothAdapter;
    private BluetoothSocket socket;
    private OutputStream outputStream;
    private InputStream inputStream;
    private TextView textView;
    private Button startButton, upButton, downButton, leftButton, rightButton, stopButton;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        startButton = (Button)findViewById(R.id.buttonStart);
        upButton = (Button)findViewById(R.id.buttonUp);
        downButton = (Button)findViewById(R.id.buttonDown);
        leftButton = (Button)findViewById(R.id.buttonLeft);
        rightButton = (Button)findViewById(R.id.buttonRight);
        stopButton = (Button)findViewById(R.id.buttonStop);
        textView = (TextView)findViewById(R.id.textView);

        startButton.setEnabled(true);
        upButton.setEnabled(true);
        downButton.setEnabled(true);
        leftButton.setEnabled(true);
        rightButton.setEnabled(true);
        stopButton.setEnabled(true);
        textView.setEnabled(true);
    }

    public void onClickStart(View view)
    {
        // get the adapter
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter == null)
        {
            //Show a mensag. that thed evice has no bluetooth adapter
            Toast.makeText(getApplicationContext(), "Bluetooth Device Not Available", Toast.LENGTH_SHORT).show();
            //finish apk
            finish();
        }
        else
        {
            if(!(bluetoothAdapter.isEnabled()))
            {
                //Ask to the user turn the bluetooth on
                Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(turnBTon,1);
            }
            Set<BluetoothDevice> bondedDevices = bluetoothAdapter.getBondedDevices();
            if(bondedDevices.isEmpty())
            {
                Toast.makeText(getApplicationContext(),"Please Pair the Device first",Toast.LENGTH_SHORT).show();
            }
            else
            {
                // checks all paired devices for the HC-05 module
                for (BluetoothDevice iterator : bondedDevices)
                {
                    if(iterator.getName().equals(DEVICE_NAME))
                    {
                        device = iterator; //device is an object of type BluetoothDevice
                        break;
                    }

                }
                ParcelUuid[] uuids = device.getUuids();
                // create a socket to handle to connection
                try
                {
                    socket = device.createRfcommSocketToServiceRecord(uuids[0].getUuid());
                }
                catch(IOException e)
                {
                    e.printStackTrace();
                }
                try
                {
                    socket.connect();
                    outputStream = socket.getOutputStream(); // data forwards
                    inputStream = socket.getInputStream(); // data received
                }
                catch(IOException e)
                {
                    e.printStackTrace();
                }
            }
        }
    }

    public void onClickDown(View view)
    {
        try
        {
            outputStream.write("d".getBytes());
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }

    public void onClickUp(View view)
    {
        try
        {
            outputStream.write("u".getBytes());
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }

    public void onClickRight(View view)
    {
        try
        {
            outputStream.write("r".getBytes());
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }

    public void onClickLeft(View view)
    {
        try
        {
            outputStream.write("l".getBytes());
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }

    public void onClickStop(View view)
    {
//        stopThread = true;
        try
        {
            outputStream.close();
            inputStream.close();
            socket.close();
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }
}
