#include "libs/BasicTree.h"

using prglib::BasicTree;

 BasicTree::BasicTree() {
  esq = nullptr;
  dir = nullptr;
  h = 0;
  dado = nullptr;
}

 BasicTree::BasicTree(void * p_dado) {
  esq = nullptr;
  dir = nullptr;
  h = 0;
  dado = p_dado;
}

 
 /*BasicTree * BasicTree::create(void* p_dado) {
     return new BasicTree(p_dado);
 }*/
 
 /*BasicTree::BasicTree(const BasicTree & outra) : h(outra.h){
  dado = outra.dado;

  if (outra.dir) dir = this->create(outra.dir->dado);
  else dir = nullptr;

  if (outra.esq) esq = this->create(outra.esq->dado);
  else esq = nullptr;
}*/

 BasicTree::~BasicTree() {
  if (esq) delete esq;
  if (dir) delete dir;
}

 unsigned int BasicTree::tamanho() const {
    unsigned int r = 1;
    
    if (esq) r += esq->tamanho();
    if (dir) r += dir->tamanho();
    
    return r;
}

 void BasicTree::adiciona(void * algo) {
  BasicTree * ptr = this;

  while (true) {
    if (this->ehIgual(ptr->dado, algo)) {
      this->atribui(ptr->dado, algo);
      break;
    }
    ptr->h = -1;
    if (this->ehMenor(algo, ptr->dado)) {
      if (ptr->esq) ptr = ptr->esq;
      else {
        ptr->esq = this->create(algo);
        break;
      }
    } else {
      if (ptr->dir) ptr = ptr->dir;
      else {
        ptr->dir = this->create(algo);
        break;
      }
    }
  }
}


 void * BasicTree::obtem(void * algo) const {
   BasicTree * nulo;
   BasicTree * ptr = (BasicTree*)this;
   ptr = ptr->obtem_nodo(algo, nulo);

  if (not ptr) throw -1;
  
  return ptr->dado;
}

 void BasicTree::listeEmLargura(list<void*> & result) {
     list<BasicTree*> q;
     
     q.push_back(this);
     while (not q.empty()) {
         BasicTree * ptr = q.front();
         q.pop_front();
         result.push_back(ptr->dado);
         if (ptr->esq) q.push_back(ptr->esq);
         if (ptr->dir) q.push_back(ptr->dir);
     }
 }
 
 void BasicTree::listeInOrder(list<void*> & result) {
  if (esq) esq->listeInOrder(result);
  result.push_back(dado);
  if (dir) dir->listeInOrder(result);
}

 void BasicTree::listePreOrder(list<void*> & result) {
  result.push_back(dado);
  if (esq) esq->listePreOrder(result);
  if (dir) dir->listePreOrder(result);
}

 void BasicTree::listePostOrder(list<void*> & result) {
  if (esq) esq->listePostOrder(result);
  if (dir) dir->listePostOrder(result);
  result.push_back(dado);
}

 unsigned int BasicTree::altura() {    
    if (h >= 0) return h;

    //if (not esq and not dir) return 0;
    
    unsigned int hd = 0, he = 0;
    
    if (esq) he = 1 + esq->altura();
    if (dir) hd = 1 + dir->altura();
    
    if (hd > he) h = hd;
    else h = he;
    
    return h;
}

 int BasicTree::fatorB() {
    unsigned int he=0, hd=0;
    
    if (esq) he = 1 + esq->altura();
    if (dir) hd = 1 + dir->altura();
    
    return he - hd;
}

// balanceamento
 BasicTree * BasicTree::rotacionaL() {
    BasicTree * p3 = this;
    BasicTree * p2 = p3->esq;
    BasicTree * c = p2->dir;
    
    p3->esq = c;
    //if (c) c->pai = p3;
    p2->dir = p3;
    //p3->pai = p2;
    //p2->pai = nullptr; // nova raiz
    
    p2->h = -1;
    p3->h = -1;
    return p2;
}

 BasicTree * BasicTree::rotacionaR() {
    BasicTree * p1 = this;
    BasicTree * p2 = p1->dir;
    BasicTree * b = p2->esq;
    
    p1->dir = b;
    //if (b) b->pai = p1;
    p2->esq = p1;
    //p1->pai = p2;
    //p2->pai = nullptr; // nova raiz
    
    p2->h = -1;
    p1->h = -1;
    return p2;    
}

 BasicTree * BasicTree::balanceia() {
    if (not (esq or dir)) return this;
    if (esq) esq = esq->balanceia();
    if (dir) dir = dir->balanceia();
    
    int fb = fatorB();
    BasicTree * ptr = this;
    while (fb < -1) {
        if (ptr->dir->fatorB() > 0) ptr->dir = ptr->dir->rotacionaL();
        ptr = ptr->rotacionaR();
        fb = ptr->fatorB();
    }
    while (fb > 1) {
        if (ptr->esq->fatorB() < 0) ptr->esq = ptr->esq->rotacionaR();
        ptr = ptr->rotacionaL();
        fb = ptr->fatorB();        
    }
    ptr->h = -1;
    return ptr;
}

 BasicTree * BasicTree::balanceia(bool otimo) {
    unsigned int h;

    if (otimo) h  = altura();
    
    BasicTree * ptr = balanceia();

    if (not otimo) return ptr;
    
    unsigned int ho = h;
    
    while ((h = ptr->altura()) < ho) {
        ptr = ptr->balanceia();
        ho = h;
    }
    
    return ptr;
}

void BasicTree::inicia() {
    q.clear();
    //pilha.adiciona(raiz);
    this->desce(this);
}

void BasicTree::rinicia() {
    rq.clear();
    //pilha.adiciona(raiz);
    this->rdesce(this);
}

 bool BasicTree::fim() {
    return q.empty();
}

bool BasicTree::rfim() {
    return rq.empty();
}

/* T& BasicTree::Iterador::proximo() {
    if (pilha.vazia()) throw -1;
    BasicTree * ptr = pilha.remove(0);
    if (ptr->dir) pilha.adiciona(ptr->dir);
    if (ptr->esq) pilha.adiciona(ptr->esq);
    return ptr->dado;
}*/

void BasicTree::desce(BasicTree * ptr) {
    while (ptr) {
        q.push_front(ptr);
        ptr = ptr->esq;
    }
}

void BasicTree::rdesce(BasicTree * ptr) {
    while (ptr) {
        rq.push_front(ptr);
        ptr = ptr->dir;
    }
}

 void* BasicTree::proximo() {
    if (q.empty()) throw -1;
    BasicTree * ptr = q.front();
    q.pop_front();
    if (ptr->dir) desce(ptr->dir);
    return ptr->dado;
}

void* BasicTree::rproximo() {
    if (rq.empty()) throw -1;
    BasicTree * ptr = rq.front();
    rq.pop_front();
    if (ptr->esq) this->rdesce(ptr->esq);
    return ptr->dado;
}

  void* BasicTree::obtemMaior() const {
    const BasicTree * ptr = this;
    
    while (ptr->dir) ptr = ptr->dir;
    return ptr->dado;
}

  void* BasicTree::obtemMenor() const {
    const BasicTree * ptr = this;
    
    while (ptr->esq) ptr = ptr->esq;
    return ptr->dado;
}

 BasicTree * BasicTree::obtem_nodo(void * algo, BasicTree* & pai) {
  BasicTree * ptr = this;
  pai = nullptr;
  
  while (ptr != nullptr) {
    if (this->ehIgual(ptr->dado, algo)) return ptr;
    pai = ptr;
    if (this->ehMenor(algo, ptr->dado)) ptr = ptr->esq;
    else ptr = ptr->dir;
  }
  return nullptr;
}

  void BasicTree::remove(void * algo, void * result) {
  BasicTree * pai;
  BasicTree * ptr = obtem_nodo(algo, pai);

  if (not ptr) throw -1;

  if (result) this->atribui(result, ptr->dado);

  if (ptr->esq or ptr->dir) {
    int fb = ptr->fatorB();
    ptr->h = -1;
    if (fb > 0) { // maior do lado esquerdo
        BasicTree * maior = this->create(ptr->esq->obtemMaior());
        ptr->remove(maior->dado, nullptr);             
        this->atribui(ptr->dado, maior->dado);
        delete maior;
    } else { // maior do lado direito
        BasicTree * menor = this->create(ptr->dir->obtemMenor());
        ptr->remove(menor->dado, nullptr);             
        this->atribui(ptr->dado, menor->dado);
        delete menor;
    }
  } else { // nodo é uma folha
    if (pai) {
        if (pai->esq == ptr) pai->esq = nullptr;
        else pai->dir = nullptr;
    }
    delete ptr;
  }
}

 void BasicTree::obtemMenoresQue(list<void*>& result, void* algo) {
    if (this->ehMenor(dado,algo)) {
        result.push_back(dado);
        if (esq) esq->listePreOrder(result);
        if (dir) dir->obtemMenoresQue(result, algo);
    } else {
        if (esq) esq->obtemMenoresQue(result, algo);
    }
}

 void BasicTree::obtemMaioresQue(list<void*>& result, void* algo) {
    if (this->ehMenor(algo,dado)) {
        result.push_back(dado);
        if (dir) dir->listePreOrder(result);
        if (esq) esq->obtemMaioresQue(result, algo);
    } else {
        if (dir) dir->obtemMaioresQue(result, algo);
    }
}

 void BasicTree::obtemIntervalo(list<void*>& result, void* start, void* end) {
    if ((this->ehIgual(start, dado) or this->ehMenor(start, dado)) and
         (this->ehIgual(end, dado) or this->ehMenor(dado, end))) {
        result.push_back(dado);
        if (esq) esq->obtemIntervalo(result, start, end);
        if (dir) dir->obtemIntervalo(result, start, end);
    } else if (this->ehMenor(dado, start) or this->ehIgual(dado, start)) {
        if (dir) dir->obtemIntervalo(result, start, end);
    } else {
        if (esq) esq->obtemIntervalo(result, start, end);
    }
     
 }
