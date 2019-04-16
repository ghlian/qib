# A Q wrapper for the Interactive Brokers API

Q Wrapper around the latest TWS C++ API v9.74.01.
Forms the basis for applications integrating with Interactive Brokers, such as:

- feedhandler process to capture market data in a kdb+tick database
- order management system
- strategy testing and running

For review of supported functionality, refer to [./test/test_qib.q](test/test_qib.q) in conjunction with [interactivebrokers.github.io docs](https://interactivebrokers.github.io/tws-api/index.html)
