var request = require('request');

module.exports = exports = (function () {

    function PushoverHandler (url, delay) {
        var self = this;

        // The collection of messages to send to Pushover
        self.messages = [];
        self.lastSentTime = undefined;
        self.url = url;
        self.messageDelayTime = delay;
        self.messageTimer = undefined;

        self.handleRequest = function (req, res) {
            var token = req.body.token,
                message = req.body.message,
                user = req.body.user;

            if (!token || !message || !user) {
                res.status(400).json({ msg: 'A <token>, <user>, and <message> are required inputs.' });
                return;
            }

            self.messages.push({ token: token, user: user, msg: message });
            process.nextTick(function () {
                self.sendFromQueue();
            });
            res.status(200).json({ msg: 'Message recieved: ' + self.messages.length + ' in queue.' });
        };

        self.sendFromQueue = function () {
            if (self.messages.length > 0 && 
                (self.lastSentTime === undefined || Date.now() - self.lastSentTime >= self.messageDelayTime)) {
                // Time to send a message
                var msgObj = self.messages.shift();
                self.sendImmediate(msgObj);
            } else {
                self.startMessageTimer();
            }
        }

        self.startMessageTimer = function () {
            if (self.messageTimer) {
                clearTimeout(self.messageTimer);
                self.messageTimer = undefined;
            }

            if (self.messages.length > 0) {
                self.messageTimer = setTimeout(self.sendFromQueue, Date.now() - self.lastSentTime);
            }
        }

        self.sendImmediate = function (msgObj) {
            var opts = {
                url: self.url,
                method: 'POST',
                json: true,
                body: {
                    token: msgObj.token,
                    user: msgObj.user,
                    message: msgObj.msg
                }
            };

            request(opts,
                function (err, response, body) {
                    console.log('err: ', err, ' status code: ', response.statusCode, ' respones body: ', body);
                    self.lastSentTime = Date.now();
                    self.startMessageTimer();
                }
            );
        }
    }

    return PushoverHandler;

})();