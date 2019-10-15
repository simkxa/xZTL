#include <pthread.h>
#include <sys/queue.h>
#include <xapp.h>

#define ZTL_PRO_TYPES 1

enum ztl_pro_type_list {
    ZTL_PRO_TUSER = 0x0
};

struct ztl_pro_zone {
    struct xapp_maddr		addr;
    struct app_zmd_entry       *zmd_entry;
    uint8_t 			lock;
    uint8_t 			state;
    TAILQ_ENTRY (ztl_pro_zone) entry;
    TAILQ_ENTRY (ztl_pro_zone) open_entry;
};

struct ztl_pro_grp {
    struct ztl_pro_zone *vzones;
    uint32_t nfree;
    uint32_t nused;
    uint32_t nopen[ZTL_PRO_TYPES];

    TAILQ_HEAD (free_list, ztl_pro_zone) free_head;
    TAILQ_HEAD (used_list, ztl_pro_zone) used_head;

    /* Open zones for distinct provisioning types */
    TAILQ_HEAD (open_list, ztl_pro_zone) open_head[ZTL_PRO_TYPES];

    pthread_spinlock_t spin;
};

int  ztl_pro_grp_init (struct app_group *grp);
void ztl_pro_grp_exit (struct app_group *grp);
int  ztl_pro_grp_put_zone (struct app_group *grp, uint32_t zone_i);
int  ztl_pro_grp_get (struct app_group *grp, struct xapp_maddr *list,
					     uint32_t nsec, uint8_t ptype);
