#!/usr/bin/env node

var express = require('express'),
	bodyParser = require('body-parser'),
	PushoverHandler = require('./pushover-handler');

var app = express(),
	port = process.env.PORT || 67534,
	pushoverUrl = process.env.PUSHOVER_URL || 'https://api.pushover.net/1/messages.json',
	pushoverHandler = new PushoverHandler(pushoverUrl);

app.use(bodyParser.json());

app.post('/message', pushoverHandler.handleRequest);

app.all('*', function (req, res) {
	res.status(401).json({ msg: "unauthorized" });
});	

app.listen(port, function() {
  console.log("Pushover app is running at localhost:" + port)
})
