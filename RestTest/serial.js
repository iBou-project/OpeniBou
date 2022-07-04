const { SerialPort } = require('serialport')
// or
//import { SerialPort } from 'serialport'

// Create a port

const port = new SerialPort({
  path: 'COM10',
  baudRate: 115200,
})


port.write('main screen turn on', function(err) {
    if (err) {
      return console.log('Error on write: ', err.message)
    }
    console.log('message written')
  })
  
  // Open errors will be emitted as an error event
  port.on('error', function(err) {
    console.log('Error: ', err.message)
  })