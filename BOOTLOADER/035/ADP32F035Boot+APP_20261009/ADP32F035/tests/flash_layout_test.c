#include <assert.h>
#include <stdio.h>
#include "flash_layout.h"

int main(void)
{
    static const uint16_t expected[] = {0x80,0x40,0x20,0x10,8,4,2,1};
    uint16_t i, j, mask, app, metadata;
    uint32_t start, count;
    for (i=0;i<8;++i)
    {
        start=0x3E8000UL+(uint32_t)i*0x2000UL;
        assert(flash_range_mask(start,4)==expected[i]);
        assert(flash_range_mask(start+0x1FFCUL,4)==expected[i]);
        mask=0;
        for (j=i;j<8;++j)
        {
            mask|=expected[j];
            count=(uint32_t)(j-i+1)*0x2000UL;
            assert(flash_range_mask(start,count)==mask);
        }
        if(i<7) assert(flash_range_mask(start+0x1FFCUL,8)==(expected[i]|expected[i+1]));
    }
    assert(!flash_range_mask(0x3E7FFFUL,4));
    assert(!flash_range_mask(0x3F8000UL,4));
    assert(!flash_range_mask(0x3E8000UL,0));
    assert(!flash_range_mask(0x3E8000UL,0xFFFFFFFFUL));
    assert(!flash_range_mask(0xFFFFFFFCUL,8));

    /* Explicitly reproduce the old failure: a G image must not erase H. */
    assert(flash_range_mask(0x3EA000UL,4)==0x40);
    for (count=4;count<=PART_APP_END-PART_APP_START;count+=4)
    {
        uint16_t wanted=0;
        for(i=0;i<8;++i)
        {
            uint32_t physical=0x3E8000UL+(uint32_t)i*0x2000UL;
            if(physical>=PART_APP_START && physical<PART_APP_START+count)wanted|=expected[i];
        }
        assert(flash_begin_plan(PART_APP_START,count,PART_APP_START,&app,&metadata));
        assert(app==wanted);
        assert(metadata==expected[(PART_METADATA-0x3E8000UL)/0x2000UL]);
        assert(!(app&metadata));
        assert(!(app&flash_range_mask(PART_BOOT_START,PART_FLASH_END-PART_BOOT_START)));
    }
    assert(!flash_begin_plan(PART_APP_START+4,4,PART_APP_START,&app,&metadata));
    assert(!app && !metadata);
    assert(!flash_begin_plan(PART_APP_START,4,PART_APP_START+4,&app,&metadata));
    assert(!flash_begin_plan(PART_APP_START,3,PART_APP_START,&app,&metadata));
    assert(!flash_begin_plan(PART_APP_START,0,PART_APP_START,&app,&metadata));
    assert(!flash_begin_plan(PART_APP_START,0xFFFFFFFFUL,PART_APP_START,&app,&metadata));
    assert(!flash_begin_plan(PART_APP_START,PART_APP_END-PART_APP_START+4,PART_APP_START,&app,&metadata));
    assert(flash_write_allowed(PART_APP_START,4));
    assert(flash_write_allowed(PART_APP_END-4,4));
    assert(!flash_write_allowed(PART_APP_START-1,4));
    assert(!flash_write_allowed(PART_APP_END-4,8));
    assert(flash_write_allowed(PART_METADATA,20));
    assert(flash_write_allowed(PART_METADATA+18,2));
    assert(!flash_write_allowed(PART_METADATA+18,3));
    assert(!flash_write_allowed(PART_METADATA+20,4));
    assert(!flash_write_allowed(PART_BOOT_START,4));
    assert(!flash_write_allowed(PART_APP_START,0xFFFFFFFFUL));
    if(PART_APP_END<PART_METADATA) assert(!flash_write_allowed(PART_APP_END,4));
    printf("PASS layout APP=%06lX..%06lX META=%06lX BOOT=%06lX\n",
           (unsigned long)PART_APP_START,(unsigned long)PART_APP_END,
           (unsigned long)PART_METADATA,(unsigned long)PART_BOOT_START);
    return 0;
}
