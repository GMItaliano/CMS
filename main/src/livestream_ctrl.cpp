#include "livestream_ctrl.h"
#include <unistd.h>
#include <sys/wait.h>

livestream_ctrl::livestream_ctrl() 
{

    //Cdevice = cam;
    //Mdevice = micro;

    //initialize microphone

}

livestream_ctrl::~livestream_ctrl(){

}

void livestream_ctrl::start_livestream(){

    pid_t pid = fork();
    if(pid == 0){
        /*
        ffmpeg -re -f v4l2 -video_size 320x240 -thread_queue_size 16384 -framerate 70 -i /dev/video0 -f alsa -ac 1 -thread_queue_size 1024 
        -ar 44100 -i plughw:0,0 -f flv rtmp://localhost/live -c:a libfdk_aac -b:a 128k 
        -c:v libx264 -b:v 1600k -qp 0  -preset ultrafast -filter:v fps=fps=30 -tune zerolatency -x264opts keyint=50 -g 25 -pix_fmt yuv420p 
        */
        char *args[] = {"ffmpeg", "-re", "-f", "v4l2", "-video_size", "320x240", "-thread_queue_size", "16384", "-framerate", "70", "-i", "/dev/video0", "-f", "alsa", "-ac", "1", "-thread_queue_size", "1024", "-ar", "44100", "-i", "plughw:1,0", "-f", "flv", "rtmp://localhost/live", "-c:a", "libfdk_aac", "-b:a", "128k", "-c:v", "libx264", "-b:v", "1600k", "-qp", "0", "-preset", "ultrafast", "-filter:v", "fps=fps=30", "-tune", "zerolatency", "-x264opts", "keyint=50", "-g", "25", "-pix_fmt", "yuv420p", NULL};

        execvp("ffmpeg",args);

    } else {
        // Parent process
        int status;
        waitpid(pid, &status, WNOHANG);
    }	
    

    //send to NGINX
}

void livestream_ctrl::stop_livestream(){

    system("pidof ffmpeg | xargs kill -9");

    //stop sending to NGINX
}

    void livestream_ctrl::play_audio(){

        //database storage link: gs://cms-rasp.appspot.com/audio_livestream/mp3_file.mp3 / https://firebasestorage.googleapis.com/v0/b/cms-rasp.appspot.com/o/audio_livestream%252Fmp3_file.mp3?alt=media
        //sound producer used: omxplayer

        pid_t pid = fork();
        if(pid == 0){   //child process

            system("rm audio.wav audio_livestream%2Fmp3_file.mp3?alt=media && wget \"https://firebasestorage.googleapis.com/v0/b/cms-rasp.appspot.com/o/audio_livestream%2Fmp3_file.mp3?alt=media\" && ffmpeg -i audio_livestream%2Fmp3_file.mp3?alt=media audio.wav");
            //system("wget \"https://firebasestorage.googleapis.com/v0/b/cms-rasp.appspot.com/o/audio_livestream%2Fmp3_file.mp3?alt=media\"");
            //system("ffmpeg -i audio_livestream%2Fmp3_file.mp3?alt=media audio.wav");

            char *args[] = {"aplay","audio.wav", NULL};
            execvp("aplay",args);

        }else{          //parent process
            int status;
            waitpid(pid, &status, WNOHANG);
        }

    }