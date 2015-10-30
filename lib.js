//"use strict";

var LibraryDLM = {
    $gfxContext: null,
    $DLM: {
        audioCtx: null,
        audioBuffer: [],
        audioSources: [],
        bgmPlayer: null,
        currentBgm: 0,
        mirror: false,
        images: [],
        fontSize: 14,
        fontType: 0,
    },

    //------------------------------------------------------------------------------
    // Graphics
    //------------------------------------------------------------------------------
    
    graphics_init: function() {
        var canvas = Module['canvas'];
        gfxContext = canvas.getContext('2d');
        gfxContext.textBaseline = 'top';
        gfxContext.strokeStyle = 'black';
    },

    loadimage: function(filename) {
        filename = UTF8ToString(filename);
        var img = new Image();
        img.src = filename;
        DLM.images.push({
            img: img,
            x: 0,
            y: 0,
            w: 1,
            h: 1,
        });
        return DLM.images.length - 1;
    },

    subimage: function(x, y, w, h, img) {
        var src = DLM.images[img];
        DLM.images.push({
            img: src.img,
            x: x,
            y: y,
            w: w,
            h: h
        });
        return DLM.images.length - 1;
    },

    getimagesize: function(img, pw, ph) {
        var src = DLM.images[img];
        setValue(pw, src.w, 'i32');
        setValue(ph, src.h, 'i32');
    },

    clearscreen: function() {
        gfxContext.fillRect(0, 0, 480, 420);
    },

    drawline: function(x, y, w, h) {
        gfxContext.beginPath();
        gfxContext.moveTo(x, y);
        gfxContext.lineTo(w, h);
        gfxContext.closePath();
        gfxContext.stroke();
    },

    drawrect: function(x, y, w, h) {
        gfxContext.strokeRect(x, y, w, h);
    },

    fillrect: function(x, y, w, h) {
        gfxContext.fillRect(x, y, w, h);
    },

    drawarc: function(x, y, w, h) {
        gfxContext.lineWidth = 0.5;
        gfxContext.strokeStyle = 'black';
        gfxContext.arc(x, y, w, 0, Math.PI * 2);
        gfxContext.stroke();
    },

    fillarc: function(x, y, w, h) {
        gfxContext.beginPath();
        gfxContext.arc(x, y, w, 0, Math.PI * 2);
        gfxContext.closePath();
        gfxContext.fill();
    },


    drawimage: function(img, x, y) {
        var src = DLM.images[img];
        if (!src)
            return;
        
        gfxContext.fillStyle = 'white';
        if (DLM.mirror) {
            gfxContext.save();
            gfxContext.translate(x + src.w, y);
            gfxContext.scale(-1, 1);
            gfxContext.drawImage(src.img, src.x, src.y, src.w, src.h, 0, 0, src.w, src.h);
            gfxContext.restore();
        }
        else {
            gfxContext.drawImage(src.img, src.x, src.y, src.w, src.h, x, y, src.w, src.h);
        }
    },

    drawimageflip: function(img, x, y) {
        var src = DLM.images[img];
        if (!src)
            return;
        
        gfxContext.fillStyle = 'white';
        gfxContext.save();
        gfxContext.translate(x, y + src.h);
        gfxContext.scale(1, -1);
        gfxContext.drawImage(src.img, src.x, src.y, src.w, src.h, 0, 0, src.w, src.h);
        gfxContext.restore();
    },

    setfont: function (size, thick) {
        DLM.fontSize = size;
    },

    setfonttype: function(type) {
        DLM.fontType = type;
    },

    drawstring: function(x, y, str) {
        str = UTF8ToString(str);
        gfxContext.font = DLM.fontSize + 'px sans-serif';
        if (DLM.fontType == 1) {
            var f = gfxContext.fillStyle;
            gfxContext.fillStyle = 'black';
            gfxContext.fillText(str, x, y - 1);
            gfxContext.fillText(str, x, y + 1);
            gfxContext.fillText(str, x - 1, y);
            gfxContext.fillText(str, x + 1, y);
            gfxContext.fillStyle = f;

        }
        gfxContext.fillText(str, x, y);
    },

    setcolor: function(r, g, b) {
        gfxContext.fillStyle = 'rgba(' + r + ',' + g + ',' + b + ',' + 255 + ')';
    },

    setmirror: function(mirror) {
        DLM.mirror = (mirror != 0);
    },

    //------------------------------------------------------------------------------
    // Input
    //------------------------------------------------------------------------------
    input_init: function() {
        var canvas = Module['canvas'];
        var keydown_callback = function (e) {
            e.preventDefault();
            if (e.repeat)
                return;

            if (e.type == 'keydown') {
                DLM.keyboard[e.keyCode] = 1;
            }
            else if (e.type == 'keyup') {
                DLM.keyboard[e.keyCode] = 0;
            }
        };
        canvas.addEventListener('keydown', keydown_callback, true);
        canvas.addEventListener('keyup', keydown_callback, true);
        DLM.keyboard = [];
    },

    input_waitkey: function() {
    },

    input_keydown: function(key) {
        return DLM.keyboard[key] | 0;
    },

    input_getjoypad: function() {
        joypad = 0;
        var key = DLM.keyboard;
        if (key[37]) joypad |= 2;
        if (key[38]) joypad |= 8;
        if (key[39]) joypad |= 4;
        if (key[40]) joypad |= 1;

        return joypad;
    },

    getrand: function(maxValue) {
        return Math.floor(Math.random() * maxValue);
    },

    gettime: function() {
        var t = new Date().getTime();
        return t % 0xfffffff;
    },

    //------------------------------------------------------------------------------
    // Sound
    //------------------------------------------------------------------------------

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
autoAddDeps(LibraryDLM, '$DLM', '$gfxContext');
mergeInto(LibraryManager.library, LibraryDLM);
