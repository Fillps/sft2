/**
 * Testes unitários da biblioteca t2fs.
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include "logic.h"
#include "logic_extra.h"
#include "t2fs.h"

#ifdef __cplusplus
}
#endif


#include "gtest/gtest.h"



void copy(char *source, char *dest)
{
    int childExitStatus;
    pid_t pid;
    int status;
    if (!source || !dest) {
        /* handle as you wish */
    }

    pid = fork();

    if (pid == 0) { /* child */
        execl("/bin/cp", "/bin/cp", source, dest, (char *)0);
    }
    else if (pid < 0) {
        printf("\nErro ao reset no disco!\n");
        /* error - couldn't start process - you decide how to handle */
    }
    else {
        /* parent - wait for child - this has all error handling, you
         * could just call wait() as long as you are only expecting to
         * have one child process at a time.
         */
        pid_t ws = waitpid( pid, &childExitStatus, WNOHANG);
        if (ws == -1)
        { /* error - handle as you wish */
        }

        if( WIFEXITED(childExitStatus)) /* exit code in childExitStatus */
        {
            status = WEXITSTATUS(childExitStatus); /* zero is normal exit */
            /* handle non-zero as you wish */
        }
        else if (WIFSIGNALED(childExitStatus)) /* killed */
        {
        }
        else if (WIFSTOPPED(childExitStatus)) /* stopped */
        {
        }
    }
}

class T2FSTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        Init();
    }

    virtual void TearDown() {
        copy("t2fs_disk_cp.dat", "t2fs_disk.dat");//Reset no disco
    }
};


TEST_F(T2FSTest, fat_read_write){
    DeleteFile(0x10);
    getFat()->data[getFat()->size-1] = EOF_CLUSTER;
    char* original_fat = (char*)(malloc(getFat()->size * 4));
    strcpy((char*)original_fat, (char*)getFat()->data);
    FatWrite();
    InitFat();
    ASSERT_STREQ(original_fat, getFat()->data);
    free(original_fat);
}

TEST_F(T2FSTest, create_append_delete){
    int first_free = getFat()->first_free;
    int file_cluster = CreateFile();
    if (file_cluster==-1){
        printf("\nSem espaco no disco!\n");
        return;
    }
    ASSERT_EQ(first_free, file_cluster);
    ASSERT_EQ(getFat()->data[file_cluster], EOF_CLUSTER);
    first_free = getFat()->first_free;
    int appended_cluster = AppendCluster(file_cluster);
    if (appended_cluster==-1){
        printf("\nSem espaco no disco!\n");
        return;
    }
    ASSERT_EQ(first_free, appended_cluster);
    ASSERT_EQ(getFat()->data[file_cluster], appended_cluster);
    int second_append = AppendCluster(file_cluster);
    if (second_append==-1){
        printf("\nSem espaco no disco!\n");
        return;
    }
    ASSERT_EQ(getFat()->data[appended_cluster], second_append);
    DeleteFile(file_cluster);
    ASSERT_EQ(getFat()->data[file_cluster], FREE_CLUSTER);
    ASSERT_EQ(getFat()->data[appended_cluster], FREE_CLUSTER);
    ASSERT_EQ(getFat()->data[second_append], FREE_CLUSTER);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
