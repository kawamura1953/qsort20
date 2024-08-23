/***************************************************/
/*                                                 */
/*     qsort  (Quick sort function)     qs20       */
/*                                                 */
/* by �͑� �m�s (kawamura tomoyuki)    2024.02.01  */
/*       ��745-0845 �R��������s�͓���3-2          */
/*             t-kawa@crux.ocn.ne.jp               */
/***************************************************/

int _QS_SORT=0;    // ���ڃ\�[�g/�Ԑڃ\�[�g�̐ؑւ�����  0:�����I��  1:���ڃ\�[�g  2:�Ԑڃ\�[�g
int _QS_RNDM=0;    // 0:�h��Ȃ�  1:�\�[�g���Ԃ𔚑�������U���𗐐���p���Ėh�䂷��  2:��������
int _QS_MID1=120;  // �v�f��n  <= _QS_MID1 �̂Ƃ��́@3�̗v�f����s�{�b�g�����肷��i�R�_�����j
int _QS_MID2=580;  // �v�f��n  <= _QS_MID2 �̂Ƃ��́@9�̗v�f����s�{�b�g�����肷��i�X�_�����j

#include <malloc.h>
#include <time.h>
static size_t rndm; // _QS_RNDM=1 �̂Ƃ��A�Č����̂Ȃ��������Z�b�g����

#define ASS_CNT(x) {}   /* {ass_cnt += (x);} �Ƃ���΁A�v�f�̈ړ��񐔂��v���ł���*/

///////////////////�@�v�f�̈ړ��Ɋւ�����́@�ʂȂ��̂ɏ��������\�ł��@////////////////////////
#define lli    long long int
#define MV1(i) {                            a [i]=       b [i];}
#define MV4(i) {                     ((int*)a)[i]=((int*)b)[i];}
#define MV8(i) {                     ((lli*)a)[i]=((lli*)b)[i];}
#define SW1(i) {char v=       a [i];        a [i]=       b [i];        b [i]=v;}
#define SW4(i) { int v=((int*)a)[i]; ((int*)a)[i]=((int*)b)[i]; ((int*)b)[i]=v;}
#define SW8(i) { lli v=((lli*)a)[i]; ((lli*)a)[i]=((lli*)b)[i]; ((lli*)b)[i]=v;}
#define RT1(i) {char v=       a [i];        a [i]=       b [i];        b [i]=       c [i];        c [i]=v;}
#define RT4(i) { int v=((int*)a)[i]; ((int*)a)[i]=((int*)b)[i]; ((int*)b)[i]=((int*)c)[i]; ((int*)c)[i]=v;}
#define RT8(i) { lli v=((lli*)a)[i]; ((lli*)a)[i]=((lli*)b)[i]; ((lli*)b)[i]=((lli*)c)[i]; ((lli*)c)[i]=v;}

#define HIGHLOW(HIGH,LOW,MOV,WS,CC) {                                                      \
   if (HIGH) {                                                                             \
     char *e = a + HIGH;                                                                   \
     do {MOV(0) MOV(1) MOV(2) MOV(3) MOV(4) MOV(5) MOV(6) MOV(7) CC; b += 8*WS; a += 8*WS; \
     }while (a < e);                                                                       \
   }                                                                                       \
   switch ((LOW) & 7) {                                                                    \
     case 7: MOV(6)                                                                        \
     case 6: MOV(5)                                                                        \
     case 5: MOV(4)                                                                        \
     case 4: MOV(3)                                                                        \
     case 3: MOV(2)                                                                        \
     case 2: MOV(1)                                                                        \
     case 1: MOV(0)                                                                        \
     case 0: {}                                                                            \
   }                                                                                       \
} 

#define ENINT(x)  ((char*)(x) - (char*)0)
static size_t high, low;

static void   mvfnc8  ( char *a, const char *b ) {                 MV8(0)     }
static void   mvfnc4  ( char *a, const char *b ) {                 MV4(0)     }
static void   mvfnc8n ( char *a, const char *b ) {HIGHLOW(high,low,MV8,8,1==1)}
static void   mvfnc4n ( char *a, const char *b ) {HIGHLOW(high,low,MV4,4,1==1)}
static void   mvfnc1n ( char *a, const char *b ) {HIGHLOW(high,low,MV1,1,1==1)}
static void (*mvfnc)  ( char *a, const char *b );
static void   swfnc8  ( char *a,       char *b ) {                 SW8(0)     }
static void   swfnc4  ( char *a,       char *b ) {                 SW4(0)     }
static void   swfnc8n ( char *a,       char *b ) {HIGHLOW(high,low,SW8,8,1==1)}
static void   swfnc4n ( char *a,       char *b ) {HIGHLOW(high,low,SW4,4,1==1)}
static void   swfnc1n ( char *a,       char *b ) {HIGHLOW(high,low,SW1,1,1==1)}
static void (*swfnc)  ( char *a,       char *b );
static void   rtfnc8  ( char *a,       char *b,       char *c ) {                RT8(0)}
static void   rtfnc4  ( char *a,       char *b,       char *c ) {                RT4(0)}
static void   rtfnc8n ( char *a,       char *b,       char *c ) HIGHLOW(high,low,RT8,8,c+=64)
static void   rtfnc4n ( char *a,       char *b,       char *c ) HIGHLOW(high,low,RT4,4,c+=32)
static void   rtfnc1n ( char *a,       char *b,       char *c ) HIGHLOW(high,low,RT1,1,c+= 8)
static void (*rtfnc)  ( char *a,       char *b,       char *c );
static void   sbfnc8n ( char *a, char *b, size_t s ) HIGHLOW((s&(-64)),((s&(64-1))/8),SW8,8,1==1)
static void   sbfnc4n ( char *a, char *b, size_t s ) HIGHLOW((s&(-32)),((s&(32-1))/4),SW4,4,1==1)
static void   sbfnc1n ( char *a, char *b, size_t s ) HIGHLOW((s&( -8)),((s&( 8-1))/1),SW1,1,1==1)
static void (*sbfnc)  ( char *a, char *b, size_t s );

static void mmprepare( void *base, size_t siz ) {
 if       ((sizeof(lli)==8) && (ENINT(base)&(8-1))==0 && (siz&(8-1))==0) {
   high=(siz&(-64)); low=(siz&(64-1))/8;  mvfnc = (siz==8 ? mvfnc8 : mvfnc8n);
                                          swfnc = (siz==8 ? swfnc8 : swfnc8n);
                                          rtfnc = (siz==8 ? rtfnc8 : rtfnc8n);
                                          sbfnc =                    sbfnc8n;
 }else if ((sizeof(int)==4) && (ENINT(base)&(4-1))==0 && (siz&(4-1))==0) {
   high=(siz&(-32)); low=(siz&(32-1))/4;  mvfnc = (siz==4 ? mvfnc4 : mvfnc4n);
                                          swfnc = (siz==4 ? swfnc4 : swfnc4n);
                                          rtfnc = (siz==4 ? rtfnc4 : rtfnc4n);
                                          sbfnc =                    sbfnc4n;
 }else{                                                    
   high=(siz&( -8)); low=(siz&( 8-1))/1;  mvfnc = mvfnc1n;
                                          swfnc = swfnc1n;
                                          rtfnc = rtfnc1n;
                                          sbfnc = sbfnc1n;
 }
}
///////////////////�@�����܂ł��v�f�̈ړ��Ɋւ�����́@�ʂȂ��̂ɏ��������\�ł��@//////////////


///////////////////////////�@�������牺�́Aqs20�ɂ��\�[�g�v���O�����ł��@//////////////////////
#define med3(a,b,c) (CMP(a,b)<=0 ? (CMP(b,c)<=0 ? b : (CMP(a,c)<=0 ? c : a)) : \
                                   (CMP(b,c)>=0 ? b : (CMP(a,c)<=0 ? a : c)) )

#define  LT(a,b)  if ((t=CMP(a,b)) <  0) 
#define  LE(a,b)  if ((t=CMP(a,b)) <= 0) 
#define  GT(a,b)  if ((t=CMP(a,b)) >  0) 
#define  GE(a,b)  if ((t=CMP(a,b)) >= 0) 
#define  else_GT  else if (t > 0)
#define  else_LT  else if (t < 0)

#define I(x)     {x+=Esiz;}
#define D(x)     {x-=Esiz;}


/////////////////////////////////�@qsort�̕ϐ��̐錾�����̃}�N����`�@/////////////////////////////

/////////////////////////////////�@qsort�̖{�̕����̃}�N����`�@///////////////////////////////////
#define SORT_BODY( C, S, K, SORT_TYPE )                                                                   \
static void SORT_TYPE ( char *L, char *R, size_t size,  int (*cmp)(void *a, void *b) ) {                  \
 /*char *L,*R;                         �������̋�Ԃ̍��[�E�E�[�̗v�f�̐擪*/                             \
 char *l,*f,*m,*g,*r;                /*���A�����A�E ��Ԃ�ێ����邽�߂̃|�C���^*/                        \
 int  t;                             /*��Ɨp�ϐ�*/                                                       \
 size_t n,v,w;                       /*n:�������̋�Ԃ̗v�f��*/                                           \
                                                                                                          \
LOOP:                                                                                                     \
 if (R<=L) {return;}                                      /*�v�f���P�ȉ�*/                                \
                                                                                                          \
 if (L + Esiz == R) {if (CMP(L,R) > 0) S(L,R) return;}    /*�v�f���Q*/                                    \
                                                                                                          \
 n = (R - L) / Esiz + 1;                                  /*�v�f�����v�Z����*/                            \
                                                                                                          \
 if (n <= 4) {                                                                                            \
   m = R - Esiz;                                                                                          \
   LT(L,m)  GT(m,R)  LE(L,R)  S(m,R)    /*3-5-3,4*/                                                       \
                     else     K(R,m,L)  /*3-5-2*/                                                         \
            else     {}                 /*3-5-5,7 �̂Ƃ��͉������Ȃ�*/                                    \
   else_GT  GE(m,R)  S(L,R)             /*7-5-5,3*/                                                       \
            else     LE(L,R)  S(L,m)    /*7-5-7,8*/                                                       \
                     else     K(L,m,R)  /*7-5-6*/                                                         \
   else     GT(m,R)  S(L,R)             /*5-5-3*/                                                         \
            else     {}                 /*5-5-5,7 �̂Ƃ��͉������Ȃ�*/                                    \
   if (n == 4) {                                                                                          \
     char *p = L+Esiz;                                                                                    \
     LT(p,m)  GT(L,p)  S(L,p)           /*3-2-5-7*/                                                       \
              else     {}               /*3-3,4-5-7 �̂Ƃ��͉������Ȃ�*/                                  \
     else_GT  LE(p,R)  S(p,m)           /*3-6,7-5-7*/                                                     \
              else     K(p,m,R)         /*3-8-5-7*/                                                       \
     else     {}                        /*3-5-5-7 �̂Ƃ��͉������Ȃ�*/                                    \
   }                                                                                                      \
   return;                                                                                                \
 } /* n <= 4 */                                                                                           \
                                                                                                          \
                                                                                                          \
 m = L + Esiz * (n >> 1);    /*�z��̒������v�Z*/                                                         \
                                                                                                          \
 if (n <= _QS_MID1) {        /*�R�_����*/                                                                 \
   l=L; r=R; f=m-Esiz; g=m+Esiz;                                                                          \
   LT(l,m) LT(m,r)         {              I(l) D(r) goto _lfgr;} /* 3 5 7                           */    \
           else_GT LT(l,r) {S(m,r)        I(l) D(r) goto _lfgr;} /* 3 7 5                           */    \
                   else_GT {K(l,r,m)      I(l) D(r) goto _lfgr;} /* 5 7 3                           */    \
                   else    {S(m,r)             D(r) goto _5fgr;} /* 5 7 5                           */    \
           else            {S(g,r)        I(l) I(g) goto _lfgr;} /* 3 5 5          I(l) lfg5 ������ */    \
                                                                                                          \
   else_GT LT(l,r)         {S(l,m)        I(l) D(r) goto _lfgr;} /* 5 3 7                           */    \
           else_GT LT(m,r) {K(l,m,r)      I(l) D(r) goto _lfgr;} /* 7 3 5                           */    \
                   else_GT {S(l,r)        I(l) D(r) goto _lfgr;} /* 7 5 3                           */    \
                   else    {K(l,f,r)      D(f) D(r) goto _lfgr;} /* 7 5 5   S(l,r) D(r) 5fgr ������ */    \
           else            {S(l,m) S(g,r) I(l) I(g) goto _lfgr;} /* 5 3 5   S(l,m) I(l) lfg5 ������ */    \
                                                                                                          \
   else    LT(m,r)         {              D(r)      goto _5fgr;} /* 5 5 7                           */    \
           else_GT         {K(l,r,g) I(l) I(g)      goto _lfgr;} /* 5 5 3   S(l,r) I(l) lfg5 ������ */    \
           else            {S(g,r)        I(g)      goto _5fgr;} /* 5 5 5               5fg5 ������ */    \
 }                                                                                                        \
                                                                                                          \
                                                                                                          \
 if (n <= _QS_MID2) { /*�X�_����*/                                                                        \
   char *p;                                                                                               \
   if (_QS_RNDM==0) {                                                                                     \
     v=Esiz;                                                                                              \
   }else{                                                                                                 \
     v=(rndm%5+1)*Esiz;                         /*�U���h��̂��ߗ������g���ăs�{�b�g�����߂�*/            \
     rndm=(((rndm >> 1) + rndm) & 0x7FFFFFFFL); /*�U���h��̈�w�̋����̂���*/                            \
   }                                                                                                      \
   p=m-(w=v*3);     f=p+v;    g=f+v;    l=med3(p, f, g);                                                  \
           p+=w;    f+=w;     g+=w;     m=med3(p, f, g);                                                  \
           p+=w;    f+=w;     g+=w;     r=med3(p, f, g); m=med3(l, m, r);                                 \
   l=L; r=R; f=m-Esiz; g=m+Esiz;                                                                          \
   goto _lfgr;                                                                                            \
 }                                                                                                        \
                                                                                                          \
 {                        /*�Q�V�_����*/                                                                  \
 char *p,*z1,*z2,*z3;                                                                                     \
 if (_QS_RNDM==0) {                                                                                       \
   v=Esiz;                                                                                                \
 }else{                                                                                                   \
   v=(rndm%10+1)*Esiz;                        /*�U���h��̂��ߗ������g���ăs�{�b�g�����߂�*/              \
   rndm=(((rndm >> 1) + rndm) & 0x7FFFFFFFL); /*�U���h��̈�w�̋����̂���*/                              \
 }                                                                                                        \
 p=m-(w=v*3)*4;     f=p+v;    g=f+v;    z1=med3(p, f, g);                                                 \
           p+=w;    f+=w;     g+=w;     z2=med3(p, f, g);                                                 \
           p+=w;    f+=w;     g+=w;     z3=med3(p, f, g); l=med3(z1, z2, z3);                             \
           p+=w;    f+=w;     g+=w;     z1=med3(p, f, g);                                                 \
           p+=w;    f+=w;     g+=w;     z2=med3(p, f, g);                                                 \
           p+=w;    f+=w;     g+=w;     z3=med3(p, f, g); m=med3(z1, z2, z3);                             \
           p+=w;    f+=w;     g+=w;     z1=med3(p, f, g);                                                 \
           p+=w;    f+=w;     g+=w;     z2=med3(p, f, g);                                                 \
           p+=w;    f+=w;     g+=w;     z3=med3(p, f, g); r=med3(z1, z2, z3); m=med3(l, m, r);            \
 l=L; r=R; f=m-Esiz; g=m+Esiz;                                                                            \
 goto _lfgr;                                                                                              \
 }                                                                                                        \
                                                                                                          \
                                                                                                          \
/*                                                                                                        \
333...555...777�@�@lfgr ��l�̈ʒu�����r���n�߂�B357�𒙂߂�B l[fg]r�n�@ ���̌n�� l<=f g<=r ��ۏ�     \
L  l f m g r  R                                                                                           \
m�̓s�{�b�g���w���Bm�����̗v�f��1��̕����I���̒��O�܂ŁA�ύX�Ȃ��B                                       \
�u5�v�s�{�b�g�Ɠ��L�[�̗v�f��\���B�u3�v5��菬�����L�[�̗v�f��\���B�u7�v5���傫���L�[�̗v�f��\���B   \
�ul�v333�̉E�ׂ̗v�f���w�� �ur�v777�̍��ׂ̗v�f���w�� �uL�v�擪�v�f���w�� �uR�v�ŏI�v�f���w��             \
�uf�v555�̍��ׂ̗v�f���w�� �ug�v555�̉E�ׂ̗v�f���w��                                                     \
�u.�v����r�̗v�f�̗�(����0�ȏ�)��\��                                                                    \
                                                                                                          \
333355555...777 �ɂȂ����� 333355555...777 �ɂ���B333355555333...777 �Ƃ��đ�2�́u3�v�𒙂߂�B          \
   fl    g r                  f     l r               f     g  l r                                        \
   f<l �ɂȂ�����                   l=g; ����B             g��l�̊ԂɁu3�v�𒙂߂�B [fg]lr�n  r<l������ \
                                                                                                          \
333...555557777 �ɂȂ����� 333...555557777 �ɂ���B333...777555557777 �Ƃ��đ�2�́u7�v�𒙂߂�B          \
   l f    rg  R               l r     g  R            l r  f     g                                        \
          r<g �ɂȂ�����        r=f; ����B             r��f�̊ԂɁu7�v�𒙂߂�B     lr[fg]�n  r<l������ \
*/                                                                                                        \
                                                                                                          \
chk:                                          /*L l f  g r R */                                           \
 if (l<=f) if (g<=r) {          goto _lfgr;}  /*33...55...77*/                                            \
           else      {r=f;      goto _lrfg;}  /*33..5577*/                                                \
 else      if (g<=r) {l=g;      goto _fglr;}  /*3355..77*/                                                \
           else      {D(l) I(r) goto fin;  }  /*333555777*/                                               \
                                                                                                          \
chk_lf:                                                                                                   \
 if (l>f)  {l=g; goto _fglr;}    /*3355..77*/                                                             \
                                                    /*L l f  g r R*/                                      \
_lfgr:                                              /*33...55...77*/                                      \
 GT(l,m) {_7fgr: LT(r,m) {_7fg3: S(l,r) I(l) D(r) goto chk;   }  /*337..55..377*/                         \
                 else_GT {_7fg7: D(r) if (g<=r)  {goto _7fgr;}   /*337..55..777*/                         \
                                      else  {r=f; goto _7rfg;}}  /*337..55777*/                           \
                 else    {                        goto _7fg5; }} /*337..55..577*/                         \
 else_LT {_3fgr: I(l) if (l<=f) {                 goto _lfgr; }  /*333..55...77*/                         \
                      else {l=g;                  goto _fglr; }} /*33355...77*/                           \
 else    {                                        goto _5fgr;  } /*335..55...77*/                         \
                                                                                                          \
                                                                                                          \
       /*  lf  gr  */                                                                                     \
_5fgr: /*335.55..77    ��ʂ肠��*/                                                                       \
 if ((f-l+g) < (r)) goto _5fgr_g;     /* (f-l) < (r-g) �̈Ӗ� */                                          \
                                                                                                          \
_5fgr_f:                                                                                                  \
 if (l==f) {D(f) l=g;                                          goto _fglr;     }  /*33555..77*/           \
 LT(f,m)   {_53gr: S(l,f) I(l) D(f)                            goto chk_lf;    }  /*335.355..77*/         \
 else_GT   {_57gr: LT(r,m) {_57g3: K(l,r,f) I(l) D(f) D(r)     goto chk;     }    /*335.755.377*/         \
                   else_GT {_57g7: if (g==r) {r=f-Esiz;        goto _5rfg;  }     /*335.755777*/          \
                                   else      {D(r)             goto _57gr;  }}    /*335.755_777*/         \
                   else    {_57g5: if (g==r) {I(g) r=f-Esiz;   goto _5rfg;  }     /*335.755577*/          \
                                   else      {S(f,r) D(f) D(r) goto _5fgr_f;}} }  /*335.755_577*/         \
 else      {_55gr: D(f)                        /*goto _5fgr;*/ goto _5fgr_f;   }  /*335.555..77*/         \
                                                                                                          \
_5fgr_g:                                                                                                  \
 LT(g,m) {_5f3r: S(l,g) I(l) I(g)                             goto chk;     } /*335.553.77*/              \
 else_GT {_5f7r: if (g==r) {r=f;                              goto _5rfg;  }                              \
                 else LT(r,m) {_5f73: K(l,r,g) I(l) I(g) D(r) goto chk;    }  /*335.557377*/              \
                      else_GT {_5f77: D(r)                    goto _5f7r;  }                              \
                      else    {_5f75: S(g,r) I(g) D(r)        goto _5fgr_ ;}}                             \
 else    {_5f5r: I(g)  _5fgr_: if (g<=r) {    /*goto _5fgr;*/ goto _5fgr_g;}                              \
                               else      {r=f;                goto _5rfg;  }}                             \
                                                                                                          \
                                                                                                          \
       /*  l f  g r  */                                                                                   \
_7fg5: /*337..55..577    ��ʂ肠��*/                                                                     \
 if ((f-l+g) < (r)) goto _7fg5_g;     /* (f-l) < (r-g) �̈Ӗ� */                                          \
                                                                                                          \
_7fg5_f:                                                                                                  \
 if (l==f) {S(l,r) D(f) l=g; D(r)              goto _fglr; } /*33755.577*/                                \
 LT(f,m)   {_73g5: K(l,f,r) I(l) D(f) D(r)     goto chk;   } /*337.355.577*/                              \
 else_GT   {_77g5: S(f,r) D(f) D(r) if (g<=r) {goto _7fgr;}                                               \
                                    else {r=f; goto _7rfg;}} /*337.755.577*/                              \
 else      {_75g5: D(f)                        goto _7fg5; }                                              \
                                                                                                          \
_7fg5_g:                                                                                                  \
 if (g==r) {_7f5:  I(g) r=f;                   goto _7rfg; }                                              \
 LT(g,m)   {_7f35: K(l,g,r) I(l) I(g) D(r)     goto chk;   }                                              \
 else_GT   {_7f75: S(g,r) I(g) D(r) if (g<=r) {goto _7fgr;}                                               \
                                    else {r=f; goto _7rfg;}} /*337.557.577*/                              \
 else      {_7f55: I(g)                        goto _7fg5; }                                              \
                                                                                                          \
                                                                                                          \
       /*  l r f  g */                                                                                    \
_5rfg: /*335..775577*/                                                                                    \
 if (l==r) {       I(l) D(r)                                       goto fin_rlfg;}  /*�{�{3357775577*/    \
 LT(r,m)   {_53fg: {if (r==f) S(l,r) else K(l,r,f)} I(l) D(r) D(f) goto _lrfg;   }  /*      r  f  g */    \
 else_GT   {_57fg: D(r)                                            goto _5rfg;   }                        \
 else      {_55fg: {if (r!=f) S(r,f)} D(r) D(f)                    goto _5rfg;   }                        \
                                                                                                          \
_7rfg: /*337..775577*/                                                                                    \
 if (l==r) {       D(r)                         goto fin_rlfg;}  /*33775577 3375577*/                     \
 LT(r,m)   {_73fg: S(l,r) I(l) D(r)             goto _lrfg;   }  /*337.3775577*/                          \
 else_GT   {_77fg: D(r)                         goto _7rfg;   }  /*337.7775577*/                          \
 else      {_75fg: {if (r!=f) S(r,f)} D(r) D(f) goto _7rfg;   }  /*337.5775577*/                          \
                                                                                                          \
                                                                                                          \
_lrfg: /*m�̗v�f���ړ����邱�Ƃ͂Ȃ�*/                                                                    \
 if (l<r)  GT(r,m) {_l7fg: D(r)                                           goto _lrfg;}                    \
           else_LT {_l3fg: GT(l,m) {_73fG: S(l,r) I(l) D(r)               goto _lrfg;}                    \
                           else_LT {_33fg: I(l) if (l<r) {                goto _l3fg;}                    \
                                                else     {I(l)            goto fin_rlfg;}}                \
                           else    {_53fG: if (r==f) S(l,r) else K(l,r,f)                                 \
                                           I(l) D(r) D(f)                 goto _lrfg;}}                   \
           else    {_l5fg: {if (r!=f) S(r,f)} D(r) D(f)                   goto _lrfg;}                    \
 else                                                                                                     \
 if (l==r) LT(l,m) {_3fg:  I(l)                         goto fin_rlfg;}                                   \
           else_GT {_7fg:  D(r)                         goto fin_rlfg;}                                   \
           else    {_5fg:  I(l) D(r)                    goto fin_rlfg;}  /*�{�{333577555777*/             \
 else              {                                    goto fin_rlfg;}  /*       r f   g  */             \
                                                                                                          \
fin_rlfg:  /* r+Esiz == l || r+Esiz*2 == l ������ */                                                      \
 I(f);                                                                                                    \
 if ((v=f-l)<=0) {l=r; r=g; goto fin;}                                                                    \
 if ((w=g-f)==Esiz) S(l,f)                                                                                \
 else if (v>=w) {mmswapblock(l,f  ,w);}                                                                   \
 else           {mmswapblock(l,g-v,v);}                                                                   \
 l=r; r=g-v;                                                                                              \
 goto fin;                                                                                                \
                                                                                                          \
                                                                                                          \
_fglr:                                                                                                    \
 if (l<r)  LT(l,m) {_fg3r: I(l)                                           goto _fglr;}                    \
           else_GT {_fg7r: LT(r,m) {_fg73: S(l,r) I(l) D(r)               goto _fglr;}                    \
                           else_GT {_fg77: D(r) if (l<r) {                goto _fg7r;}                    \
                                                else     {D(r)            goto fin_fgrl;}}                \
                           else    {_fg75: if (g==l) S(l,r) else K(g,r,l)                                 \
                                           I(g) I(l) D(r)                 goto _fglr;}}                   \
           else    {_fg5r: {if (g!=l) S(g,l)} I(g) I(l)                   goto _fglr;}                    \
 else                                                                                                     \
 if (l==r) LT(l,m) {_fg3:  I(l)       goto fin_fgrl;}  /*            r  */                                \
           else_GT {_fg7:  D(r)       goto fin_fgrl;}  /*     f   g  l  */                                \
           else    {_fg5:  I(l) D(r)  goto fin_fgrl;}  /*�{�{33555333577*/                                \
 else              {                  goto fin_fgrl;}                                                     \
                                                                                                          \
fin_fgrl:  /* r+Esiz == l || r+Esiz*2 == l ������  */                                                     \
 I(f);                                                         /*333355533333777   3333555333335777*/     \
 if ((v=r-g+Esiz)<=0) {r=l; l=f-Esiz; goto fin;}               /*   f   g   rl        f   g   r l  */     \
 if ((w=g-f)==Esiz) S(f,r)                                                                                \
 else if (v>=w) {mmswapblock(f,r-w+Esiz,w);}                                                              \
 else           {mmswapblock(f,g       ,v);}                                                              \
 f=l; l=r-w; r=f;                                                                                         \
 goto fin;                                                                                                \
                                                                                                          \
                                                                                                          \
fin:                                                                                                      \
 if (l-L < R-r) {SORT_TYPE( L, l, Esiz, cmp);  L=r;} /*�������Ƀ\�[�g����*/                             \
 else           {SORT_TYPE( r, R, Esiz, cmp);  R=l;} /*�E�����Ƀ\�[�g����*/                             \
 goto LOOP;                                                                                               \
}



/////////////////////�@���ڃ\�[�g�p�� �}�N����W�J�@//////////////////////
#define C1(x,y)              {ASS_CNT(1)               mvfnc(x,y  )  ;}
#define S1(x,y)              {ASS_CNT(2)               swfnc(x,y  )  ;}
#define K1(x,y,z)            {ASS_CNT(3)               rtfnc(x,y,z)  ;}
#define mmswapblock(a,b,len) {ASS_CNT(((len)/Esiz)*2)  sbfnc((a),(b),(len));}
#define CMP(a,b)  cmp(a,b)
#define Esiz      size        /* Esiz �͎��ۂɃ\�[�g�����z��̗v�f�̑傫��*/

 SORT_BODY( C1, S1, K1, sort_direct )      /************* ���� �\�[�g �̖{�� *****************/



/////////////////////�@ptr_t �\�[�g�p�� �}�N����W�J�@//////////////////////
#define PT        *(char**)
#define C2(x,y)   {ASS_CNT(1)                   PT(x)=PT(y);                         }
#define S2(x,y)   {ASS_CNT(2) {char *tmp=PT(x); PT(x)=PT(y); PT(y)=tmp;             }}
#define K2(x,y,z) {ASS_CNT(3) {char *tmp=PT(x); PT(x)=PT(y); PT(y)=PT(z); PT(z)=tmp;}}
#undef  Esiz
#define Esiz  sizeof(char*)  /* Esiz �͎��ۂɃ\�[�g�����z��̗v�f�̑傫��*/

 SORT_BODY( C2, S2, K2, sort_ptr_t )      /************* ptr_t �\�[�g �̖{�� *****************/



/////////////////////�@�Ԑڃ\�[�g�p�� �}�N����W�J�@////////////////////////
#undef  CMP
#define CMP(a,b)  cmp( *(void**)(a), *(void**)(b) )

 SORT_BODY( C2, S2, K2, sort_indirect )       /************* �Ԑ� �\�[�g �̖{�� *****************/



///////////////�@�\�[�g�̓�����@���ڃ\�[�g�Eptr_t�\�[�g�E�Ԑڃ\�[�g�֐U�蕪����@////////////////
void qsort( void *base, size_t nel, size_t size,  int (*cmp)(void *a, void *b) )
    // base : �\�[�g���悤�Ƃ���z��ւ̃|�C���^
    // nel  : �z��base�̗v�f��
    // size : �z��base�̗v�f�̑傫���i�o�C�g�P�ʁj
    // cmp  : �v�f�̑召��r������֐��ւ̃|�C���^
{
 if (_QS_RNDM == 1) {
   rndm = (clock() & 0x7FFFFFFFL);   /* clock()�𗐐�������Ƃ��Ďg�p�B��������ۏ� */
   _QS_RNDM = 2;
 }

 if (_QS_SORT == 1)                                                 goto direct;
 if (_QS_SORT == 2)                                                 goto indirect;
 if (size == sizeof(char*) && (ENINT(base)&(sizeof(char*)-1)) == 0) goto ptr_type;
 if (size <   80)                                                   goto direct;
 if (nel  <   70)                                                   goto direct;

 // ���ڃ\�[�g��I�����邽�߂ɕK�v��eqcnt�́Anel��size���Ƃɕω�����B�����ɂ��eqcnt�̒l�����߂��B
 // ����nel(10000000�`100) X size(80�`6000)�ł�eqcnt�̕K�v�l�Beqcnt�����̒l�ȏ�Œ��ڃ\�[�g�����s�B
 // ����nel�� �E�̒l��size     1  2  3  4  4  5  6  7  8  8  9 10 11 12 16 20 24 28 32 36 40 44 48 52 56 60
 // ����nel�� �E�̒l��size 80 60 40 20 00 80 60 40 20 00 80 60 40 20 00 00 00 00 00 00 00 00 00 00 00 00 00
 static char eq_tab10M []={ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 7, 7, 7, 7, 7, 7};
 static char eq_tab4M  []={ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7};
 static char eq_tab2M  []={ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 7, 7, 7, 7, 7, 7, 7, 7};
 static char eq_tab1M  []={ 0, 0, 0, 0,00, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 7, 7};
 static char eq_tab400K[]={ 0, 0, 0, 0,00, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 7, 7, 7, 8, 8, 8, 8, 8, 8};
 static char eq_tab200K[]={ 0, 0, 0, 0,00, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8};
 static char eq_tab100K[]={ 0, 0, 0, 0,01, 1, 2, 2, 2, 3, 2, 2, 2, 3, 3, 7,11, 8, 9, 8, 8, 8, 8, 8, 8, 8, 8};
 static char eq_tab40K []={ 0, 0,01, 1, 1, 2, 3, 3, 3,03, 4, 4, 4, 4, 8,10,11,10,10, 9, 9, 9, 8, 8, 8, 8, 8};
 static char eq_tab20K []={ 0,01, 1, 1, 1, 2, 3, 4, 4, 5, 5, 7, 7, 7, 8,10,10,10,10,10,10, 9, 9, 9, 9, 9, 9};
 static char eq_tab10K []={ 0,01, 1, 1, 2, 2, 2, 2, 3, 4, 4, 4, 5, 6, 7, 9,10,10,10,10,10, 9, 9,10,10,10, 9};
 static char eq_tab4K  []={00, 1, 1, 2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 5, 7, 9, 8, 8, 8, 8, 8, 9, 9,10,10,10};
 static char eq_tab2K  []={00, 1, 2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8};
 static char eq_tab1K  []={00, 1, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8};
 static char eq_tab400 []={00, 2, 3, 4, 5, 5, 5, 5, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9};
 static char eq_tab200 []={ 0, 1, 3, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9,10,10,10};
 static char eq_tab100 []={ 0, 1, 3, 4, 5, 6, 6, 7, 7, 8, 8, 8, 9, 9,10,10,10,10,10,10,10,10,10,10,11,11,11};
 static char*eq_tab;           // "00"�͌�" 1"  "03"�͌�" 4"   "01"��" 1"->"00"->"01"

 if      (nel <    150) eq_tab = eq_tab100 ;
 else if (nel <    300) eq_tab = eq_tab200 ;
 else if (nel <    700) eq_tab = eq_tab400 ;
 else if (nel <   1500) eq_tab = eq_tab1K  ;
 else if (nel <   3000) eq_tab = eq_tab2K  ;
 else if (nel <   7000) eq_tab = eq_tab4K  ;
 else if (nel <  15000) eq_tab = eq_tab10K ;
 else if (nel <  30000) eq_tab = eq_tab20K ;
 else if (nel <  70000) eq_tab = eq_tab40K ;
 else if (nel < 150000) eq_tab = eq_tab100K;
 else if (nel < 300000) eq_tab = eq_tab200K;
 else if (nel < 700000) eq_tab = eq_tab400K;
 else if (nel <1500000) eq_tab = eq_tab1M  ;
 else if (nel <3000000) eq_tab = eq_tab2M  ;
 else if (nel <7000000) eq_tab = eq_tab4M  ;
 else /*  nel>=7000000*/eq_tab = eq_tab10M ;

 //eq_tab[nel,size]�̒l�����o���Beqcnt(0�`32)�����̒l�ȏ�Ȃ璼�ڃ\�[�g,����ȊO�Ȃ�Ԑڃ\�[�g
 {size_t siz = ( size> 6000 ? 6000 : size);
  int eq_low = ( siz < 1200 ? eq_tab[(siz-80)/80] : eq_tab[(siz-1200)/400+14] ); //�\�͍��E�Q����
  if (eq_low == 0 ) goto   direct;     // eqcnt�̒l���v�Z����܂ł��Ȃ� eqcnt(0�`32) >= eq_low(0)

#define VBUN 64          /* ��r����g(�y�A�[)�̐��̂Q�{==VBUN�B     nel >= VBUN �̕K�v����B*/

  //�K����32�g�̗v�f���r���āA�������g�̐��� eqcnt�i���l�L�[�̌��j�Ƃ���Beqcnt==0�`32�ɂȂ�B
  int eqcnt=0; size_t vsize=(nel/VBUN)*size; size_t half=vsize*(VBUN/2); char *ip_end=base+half;
  for (char *ip=base; ip<ip_end; ip+=vsize) if (cmp(ip,ip+half)==0) eqcnt++;

  if (eqcnt >= eq_low) goto direct; else goto indirect;
 }

   direct: mmprepare( base, size          ); sort_direct( base, (nel-1)*size         +base, size, cmp );  return; // ���ڃ\�[�g�̎��s

 ptr_type: mmprepare( base, sizeof(char*) ); sort_ptr_t ( base, (nel-1)*sizeof(char*)+base, size, cmp );  return; //ptr_t�\�[�g�̎��s

 indirect: {}
 size_t Z = nel * sizeof(char*) ;                 //Z�̓|�C���^�z��P�̃o�C�g��
 char *ptr = malloc( Z + size );                  //�|�C���^�z��ƍ�Ɨp�v�f�P�𓯎��Ɋm�ۂ���
 if (ptr == NULL) {goto direct;}                  //��Ɨ̈悪�m�ۂł��Ȃ��̂ŁA���ڃ\�[�g�����݂�
 {
  void **tp=(void**)ptr; char *ip=base, *ip_end=(char*)base+nel*size;
  for (; ip<ip_end; ip+=size, tp++) *tp=(void*)ip;  //�Ԑڃ\�[�g�ׂ̈ɁA�|�C���^�z��ptr�̗v�f���Z�b�g
 }

 mmprepare( ptr, sizeof(char*) ); sort_indirect( ptr, (nel-1)*sizeof(char*)+ptr, sizeof(char*), cmp ); // �Ԑڃ\�[�g�̎��s

 // �|�C���^�z���p���āA�{���̔z��̗v�f���ړ����Ă���I������
 mmprepare( base, size );
 void **tp;  char *ip, *kp, *tmp=ptr+Z; size_t i;   //tmp�͍�Ɨp�v�f
 // Knuth vol. 3 (2nd ed.) exercise 5.2-10.
 // tp[i]�̓��[�v�̎���(�擪)���w���Bip�͑ޔ�v�f�̌��̈ʒu,kp�͋󂫒n���w��
 for (tp = (void **)ptr, i = 0, ip = base; i < nel; i++, ip += size)
   if ((kp = tp[i]) != ip) {
     size_t j = i;
     char *jp = ip;
     C1(tmp, ip);
     do {
       size_t k = (kp - (char *) base) / size;
       tp[j] = jp;
       C1(jp, kp);
       j = k;
       jp = kp;
       kp = tp[k];
     }while (kp != ip);
     tp[j] = jp;
     C1(jp, tmp);
   }
 free( ptr );
 return;
}
