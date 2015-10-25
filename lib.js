//"use strict";

var LibraryDLM = {
    $DLM: {
        audioCtx: null,
        audioBuffer: [],
        audioSources: [],
        bgmPlayer: null,
        currentBgm: 0,
    },

    sound_init: function() {

        var audioCtx;
        try {
            audioCtx = new (window.AudioContext || window.webkitAudioContext)();
        }
        catch (e) {
        }

        if (audioCtx) {
            DLM.audioCtx = audioCtx;

            // Old Web Audio API (e.g. Safari 6.0.5) had an inconsistently named createGainNode function.
            if (typeof (audioCtx.createGain) === 'undefined') audioCtx.createGain = audioCtx.createGainNode;

            var loadAudio = function (id, url) {
                var xhr = new XMLHttpRequest();
                xhr.open('GET', url, true);
                xhr.responseType = 'arraybuffer';
                xhr.onload = function (e) {
                    audioCtx.decodeAudioData(this.response, function (buffer) {
                        DLM.audioBuffer[id] = buffer;
                    });
                };
                xhr.send();
            }

            loadAudio(1, 'bgm/se1.mp3');
            loadAudio(2, 'bgm/se2.mp3');
        }

        var player = document.createElement("AUDIO");
        if (player) {
            player.volume = 0.5;
            DLM.bgmPlayer = player;
        }
    },

    soundplay: function(x) {
        var i = 0;
        var s = 0;
        var d = 0;
        switch (x) {
            case 1: i = 1; s = 0.000; d = 0.641; break;
            case 3: i = 1; s = 0.700; d = 0.432; break;
            case 4: i = 1; s = 1.200; d = 0.928; break;
            case 5: i = 1; s = 2.200; d = 0.458; break;
            case 6: i = 1; s = 2.700; d = 0.249; break;
            case 7: i = 1; s = 3.000; d = 0.928; break;
            case 8: i = 1; s = 4.000; d = 0.928; break;
            case 9: i = 1; s = 5.000; d = 0.928; break;
            case 10: i = 1; s = 6.000; d = 0.432; break;
            case 11: i = 1; s = 6.500; d = 6.936; break;
            case 12: i = 1; s = 13.500; d = 3.253; break;

            case 13: i = 2; s = 0.000; d = 0.275; break;
            case 14: i = 2; s = 0.300; d = 0.118; break;
            case 15: i = 2; s = 0.500; d = 0.797; break;
            case 16: i = 2; s = 1.400; d = 4.428; break;
            case 17: i = 2; s = 5.900; d = 6.936; break;
            case 18: i = 2; s = 13.000; d = 1.476; break;
        }

        if (i > 0) {
            var audioCtx = DLM.audioCtx;
            if (audioCtx == null)
                return;

            var source = audioCtx.createBufferSource();
            source.buffer = DLM.audioBuffer[i];
            source.connect(audioCtx.destination);
            source.start(0, s, d);
            source.onended = function () {
                if (DLM.audioSources[x] == source) {
                    DLM.audioSources[x] = null;
                }
            }
            DLM.audioSources[x] = source;
        }
    },

    soundstop: function(x) {
        var s = DLM.audioSources[x];
        if (s) {
            s.stop();
            DLM.audioSources[x] = null;
        }
    },

    soundcheck: function(x) {
        return DLM.audioSources[x] != null;
    },

    bgmchange: function (x) {
        DLM.currentBgm = x;
    },

    bgmstop: function (x) {
        if (DLM.bgmPlayer) {
            DLM.bgmPlayer.pause();
        }
    },

    bgmplay: function(x) {
        var player = DLM.bgmPlayer;
        if (!player) return;

        switch (DLM.currentBgm) {
            case 100: player.src = "bgm/field.mp3"; break;
            case 103: player.src = "bgm/dungeon.mp3"; break;
            case 104: player.src = "bgm/star4.mp3"; break;
            case 105: player.src = "bgm/castle.mp3"; break;
            case 106: player.src = "bgm/puyo.mp3"; break;
            default: return;
        }

        player.loop = true;
        player.play();
    }
};
autoAddDeps(LibraryDLM, '$DLM');
mergeInto(LibraryManager.library, LibraryDLM);
