import { useEffect, useState } from 'react'
import './App.css'
import Header from './components/Header/Header';
import Sidebar from './components/Sidebar/Sidebar';
import Dashboard from './components/Dashboard/Dashboard';

function App() {
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    const getContent = async (path, setContent) => {
        try {
            const response = await axiosRq.get(`/${path}/`);
            console.log(response.data);
            setContent(response.data);
        } catch (error) {
            console.log(error);
        }
    }
    // getContent('games', setGames);
    // getContent('locations', setLocations);
    setLoading(false);
}, []);


  return (
    <div className='main-container'>
      {/* <Sidebar
        sidebarOptions={sidebarOptions}
        optionSelected={optionSelected}
        setOptionSelected={setOptionSelected}
        /> */}
      {loading ?
        <div className='loading'>
          <p>Loading...</p>
        </div>
        :   
        <div>
          {/* <Header
            duelSelected={duelSelected}
            setDuelSelected={setDuelSelected}
            duels={duels}
            games={games}
            /> */}
          <section className='main-content'>
            <Routes>
              <Route index element={
                <Dashboard
                  duel={duelSelected}
                />
              } />
              {/* <Route path='locations' element={
                <ContentForm title="locations" Card={LocationCard} />
              } />

              <Route path='events' element={
                <ContentForm title="events" Card={EventCard} />
              } />

              <Route path='games'>
                <Route index element={
                  <ContentForm title='games' Card={GameCard}>
                    
                  </ContentForm>
                }/>
              </Route> */}

            </Routes>
          </section>
        </div>
      }
    </div>
  )
}

export default App
