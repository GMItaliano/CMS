#ifndef SPEAKER_SYS_H
#define SPEAKER_SYS_H

class speaker_sys{
    private:
        bool speaker_flag;

    public:
        speaker_sys();
        ~speaker_sys();

        void emit_audio(void*);
        void volume(int);

};

#endif 